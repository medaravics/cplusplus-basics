#ifndef DB_UTIL_H
#define DB_UTIL_H

#include "PgConnetionPool.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <stdexcept>

class DbUtil {
public:
    using Row = std::unordered_map<std::string, std::string>;
    using ResultSet = std::vector<Row>;

    // Initialize singleton pool (must be called once at startup)
    static void init(const PgConnectionPool::Config& config);

    // Access pool directly (for advanced use in other classes)
    static PgConnectionPool& getPool();

    // Static utility methods (no object needed)
    static int executeNonQuery(const std::string& query);
    static ResultSet executeQuery(const std::string& query);
		
    static std::string executeScalar(const std::string& query);
    static int getAValue(const std::string& query);
		
    static int executeScalar(const std::string& query, std::vector<std::string>& value);
    static int getRowCount(const std::string& query);
    static bool isAlive();
		
		static void prepareStatement(const std::string& stmtName,const std::string& query,const int  paramsSize);
		
    static void executePrepared(const std::string& stmtName,const std::string& query,const std::vector<std::string>& params,std::vector<std::vector<std::string>>& resultTable,int& affectedRows);
		
    static std::string formatQuery(const std::string& query, const std::vector<std::string>& params);
private:
    static std::unique_ptr<PgConnectionPool> pool_;
    static std::once_flag initFlag_;

    DbUtil() = default; // hide constructor
};

#endif // DB_UTIL_H




