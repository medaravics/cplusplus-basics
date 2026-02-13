#ifndef PG_CONNECTION_POOL_H
#define PG_CONNECTION_POOL_H
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
#include <memory>
#include <chrono>
#include <vector>
#include <thread>
#include <unordered_map>

class PgConnectionPool {
public:
    // Configuration struct for pool settings
    struct Config {
        std::string conn_string;
        int min_connections;
        int max_connections;
        int increment;
        std::chrono::seconds connection_timeout;
        std::chrono::seconds idle_timeout;
        int max_retries;
        std::chrono::milliseconds retry_delay;
	std::string db_schema;
    };

    // Connection statistics
    struct Stats {
        int open_connections;
        int busy_connections;
        int free_connections;
        size_t approx_memory_usage_bytes;
    };

    // RAII-based connection handle
    class Connection {
    public:
        Connection(PgConnectionPool& pool, PGconn* conn) : pool_(pool), conn_(conn) {
            pool_.incrementBusyCount();
        }
        ~Connection() { if (conn_) pool_.releaseConnection(conn_); }
        PGconn* get() const { return conn_; }

        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        Connection(Connection&& other) : pool_(other.pool_), conn_(other.conn_) {
            other.conn_ = nullptr;
        }
        Connection& operator=(Connection&& other) {
            if (this != &other) {
                if (conn_) pool_.releaseConnection(conn_);
                conn_ = other.conn_;
                other.conn_ = nullptr;
            }
            return *this;
        }

    private:
        PgConnectionPool& pool_;
        PGconn* conn_;
    };

    PgConnectionPool(const Config& config)
        : config_(config), stop_(false), open_connections_(0), busy_connections_(0) {
        if (config_.min_connections < 1 || config_.max_connections < config_.min_connections ||
            config_.increment < 1 || config_.increment > config_.max_connections - config_.min_connections) {
            throw std::invalid_argument("Invalid pool size configuration");
        }
        initializePool();
        startIdleTimeoutThread();
    }

    ~PgConnectionPool() {
        stop_.store(true);
        condition_.notify_all();
        if (idle_thread_.joinable()) {
            idle_thread_.join();
        }
        clearPool();
    }

    std::unique_ptr<Connection> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
    	//std::cout <<"Inside getConnection\n" <<std::endl;    
        for (int attempt = 0; attempt < config_.max_retries; ++attempt) {
            if (condition_.wait_for(lock, config_.connection_timeout, 
                [this] { return !connections_.empty() || open_connections_ < config_.max_connections || stop_; })) {
                if (stop_) {
                    throw std::runtime_error("Connection pool is shutting down");
                }

                PGconn* conn;
                if (!connections_.empty()) {
                    conn = connections_.front();
                    connections_.pop();
                    last_activity_[conn] = std::chrono::steady_clock::now();
                } else if (open_connections_ < config_.max_connections) {
                    // Create connections in increments
                    int connections_to_create = std::min(config_.increment, config_.max_connections - open_connections_);
                    for (int i = 0; i < connections_to_create; ++i) {
                        conn = createConnection();
                        if (!conn) {
                            throw std::runtime_error("Failed to create new connection");
                        }
                        connections_.push(conn);
                        last_activity_[conn] = std::chrono::steady_clock::now();
                        open_connections_++;
                    }
                    conn = connections_.front();
                    connections_.pop();
                    updateMemoryUsage();
                } else {
                    continue; // Retry if no connections available
                }

                // Verify connection health
                if (PQstatus(conn) != CONNECTION_OK) {
                    PQfinish(conn);
                    last_activity_.erase(conn);
                    open_connections_--;
                    updateMemoryUsage();
                    conn = createConnection();
                    if (!conn) {
                        throw std::runtime_error("Failed to recreate connection");
                    }
                    last_activity_[conn] = std::chrono::steady_clock::now();
                    open_connections_++;
                    updateMemoryUsage();
                }

                return std::unique_ptr<Connection>(new Connection(*this, conn));
            }
            std::this_thread::sleep_for(config_.retry_delay);
        }

        throw std::runtime_error("No available connections after retries");
    }

    Stats getStats() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return {
            open_connections_,
            busy_connections_,
            static_cast<int>(connections_.size()),
            approx_memory_usage_
        };
    }

private:
    Config config_;
    std::queue<PGconn*> connections_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    std::atomic<int> open_connections_;
    std::atomic<int> busy_connections_;
    size_t approx_memory_usage_ = 0;
    std::unordered_map<PGconn*, std::chrono::steady_clock::time_point> last_activity_;
    std::thread idle_thread_;

    void initializePool() {
        for (int i = 0; i < config_.min_connections; ++i) {
            PGconn* conn = createConnection();
            if (conn) {
                connections_.push(conn);
                last_activity_[conn] = std::chrono::steady_clock::now();
                open_connections_++;
                updateMemoryUsage();
            } else {
                throw std::runtime_error("Failed to initialize connection pool");
            }
        }
    }

    PGconn* createConnection() {
        PGconn* conn = PQconnectdb(config_.conn_string.c_str());
        if (PQstatus(conn) != CONNECTION_OK) {
	    std::cerr << "Connection failed: " << PQerrorMessage(conn) << std::endl;
            PQfinish(conn);
            return nullptr;
        }
       
	if (!config_.db_schema.empty()) {
        std::string query = "SET search_path TO " + config_.db_schema;
        PGresult* res = PQexec(conn, query.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Failed to set search_path: " << PQerrorMessage(conn) << std::endl;
        }
        PQclear(res);
    }
        return conn;
    }

    void releaseConnection(PGconn* conn) {
        if (!conn) return;

        std::lock_guard<std::mutex> lock(mutex_);
        busy_connections_--;
        last_activity_[conn] = std::chrono::steady_clock::now();
        if (stop_) {
            PQfinish(conn);
            last_activity_.erase(conn);
            open_connections_--;
            updateMemoryUsage();
        } else if (PQstatus(conn) == CONNECTION_OK) {
            connections_.push(conn);
        } else {
            PQfinish(conn);
            last_activity_.erase(conn);
            open_connections_--;
            if (open_connections_ < config_.min_connections) {
                PGconn* new_conn = createConnection();
                if (new_conn) {
                    connections_.push(new_conn);
                    last_activity_[new_conn] = std::chrono::steady_clock::now();
                    open_connections_++;
                }
                updateMemoryUsage();
            }
        }
        condition_.notify_one();
    }

    void clearPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!connections_.empty()) {
            PGconn* conn = connections_.front();
            connections_.pop();
            PQfinish(conn);
            last_activity_.erase(conn);
            open_connections_--;
        }
        updateMemoryUsage();
    }

    void incrementBusyCount() {
        busy_connections_++;
    }

    void updateMemoryUsage() {
        // Approximate memory usage: ~100KB per PGconn (libpq internal structures)
        approx_memory_usage_ = open_connections_ * 100 * 1024;
    }

    void startIdleTimeoutThread() {
        if (config_.idle_timeout.count() > 0) {
            idle_thread_ = std::thread([this] {
                while (!stop_) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::lock_guard<std::mutex> lock(mutex_);
                    auto now = std::chrono::steady_clock::now();
                    std::queue<PGconn*> temp_queue;
                    while (!connections_.empty()) {
                        PGconn* conn = connections_.front();
                        connections_.pop();
                        if (now - last_activity_[conn] > config_.idle_timeout && open_connections_ > config_.min_connections) {
                            PQfinish(conn);
                            last_activity_.erase(conn);
                            open_connections_--;
                        } else {
                            temp_queue.push(conn);
                        }
                    }
                    connections_ = std::move(temp_queue);
                    updateMemoryUsage();
                }
            });
        }
    }

    // Prevent copying
    PgConnectionPool(const PgConnectionPool&) = delete;
    PgConnectionPool& operator=(const PgConnectionPool&) = delete;
};
#endif // PG_CONNECTION_POOL_H
