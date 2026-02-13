#include "DbUtil.h"
#include <iostream>
#include <stdexcept>

// Define static members
std::unique_ptr<PgConnectionPool> DbUtil::pool_;
std::once_flag DbUtil::initFlag_;

// Initialize singleton pool (thread-safe)
void DbUtil::init(const PgConnectionPool::Config& config) {
    std::call_once(initFlag_, [&]() {
        try {
            pool_.reset(new PgConnectionPool(config));
        } catch (const std::exception& ex) {
            std::cerr << "[DbUtil::init] Failed to initialize connection pool: "
                      << ex.what() << std::endl;
            std::exit(EXIT_FAILURE); // binary down immediately
        } catch (...) {
            std::cerr << "[DbUtil::init] Unknown error during initialization" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    });
}

// Access pool
PgConnectionPool& DbUtil::getPool() {
    if (!pool_) {
        throw std::runtime_error("DbUtil::init() must be called before using the pool");
    }
    return *pool_;
}

// Execute INSERT/UPDATE/DELETE
int DbUtil::executeNonQuery(const std::string& query) {
    char *affected_rows_str;
    int affected_rows=-1;
    try {
        auto conn = pool_->getConnection();
        PGresult* res = PQexec(conn->get(), query.c_str());
        if (!res) {
            throw std::runtime_error("Null PGresult returned from PQexec");
        }

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::string err = PQerrorMessage(conn->get());
            PQclear(res);
            throw std::runtime_error("Execution failed: " + err);
        }else{
	    affected_rows_str = PQcmdTuples(res);
            affected_rows = atoi(affected_rows_str); // Convert string to integer
	}

        PQclear(res);
    } catch (const std::exception& ex) {
        std::cerr << "[DbUtil::executeNonQuery] Exception: " << ex.what() << std::endl;
    }
    return affected_rows;
}

// Execute SELECT and return rows
DbUtil::ResultSet DbUtil::executeQuery(const std::string& query) {
    ResultSet result;
    try {
        auto conn = pool_->getConnection();
        PGresult* res = PQexec(conn->get(), query.c_str());
        if (!res) {
            throw std::runtime_error("Null PGresult returned from PQexec");
        }

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::string err = PQerrorMessage(conn->get());
            PQclear(res);
            throw std::runtime_error("Query failed: " + err);
        }

        int rows = PQntuples(res);
        int cols = PQnfields(res);
        
	

        for (int i = 0; i < rows; i++) {
            Row row;
            for (int j = 0; j < cols; j++) {
                char* value = PQgetvalue(res, i, j);
                row[PQfname(res, j)] = value ? value : "";
            }
            result.push_back(std::move(row));
        }

        PQclear(res);
    } catch (const std::exception& ex) {
        std::cerr << "[DbUtil::executeQuery] Exception: " << ex.what() << std::endl;
    }
    return result;
}

// Execute query and return single value
/*std::string DbUtil::executeScalar(const std::string& query) {
    try {
        auto conn = pool_->getConnection();
        PGresult* res = PQexec(conn->get(), query.c_str());
        if (!res) {
            throw std::runtime_error("Null PGresult returned from PQexec");
        }

        if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
            std::string err = PQerrorMessage(conn->get());
            PQclear(res);
            throw std::runtime_error("Scalar query failed: " + err);
        }

        std::string value = PQgetvalue(res, 0, 0);
        PQclear(res);
        return value;
    } catch (const std::exception& ex) {
        std::cerr << "[DbUtil::executeScalar] Exception: " << ex.what() << std::endl;
        return "";
    }
}*/


std::string DbUtil::executeScalar(const std::string& query) {
  std::vector<std::string> v;
  DbUtil::executeScalar(query, v);
  return v[0];
}
int DbUtil::getAValue(const std::string& query){
   int returnValue = -999; // Return -999 on exception
   try {
      returnValue=stol(executeScalar(query));
   } catch (const std::exception& ex) {
       std::cerr << __FUNCTION__ << "::Exception:[" << __LINE__ << "]" << ex.what() << std::endl;
   }
   return returnValue;

}

int DbUtil::executeScalar(const std::string& query, std::vector<std::string>& resultVector) {
    int rows=-1;
    try {
        auto conn = pool_->getConnection();
        PGresult* res = PQexec(conn->get(), query.c_str());
        if (!res) {
            throw std::runtime_error("Null PGresult returned from PQexec");
        }
        
	ExecStatusType status = PQresultStatus(res);


        if (status != PGRES_TUPLES_OK) {
            std::string errMsg = PQerrorMessage(conn->get());
            PQclear(res);
            throw std::runtime_error("Query did not return tuples. PostgreSQL error: " + errMsg);
        }

        rows = PQntuples(res);
        int cols = PQnfields(res);


        if (rows == 0) {
            PQclear(res);
            throw std::runtime_error("ROWS_NOT_FOUND");
        }
        //RCS: I assume that this case should never occur 
        if (cols == 0) {
            PQclear(res);
            throw std::runtime_error("Query returned rows but no columns.");
        }


        // Extract the first column of the first row
        char* value = PQgetvalue(res, 0, 0);
        if (value) {
            resultVector.push_back(value);
        } else {
            PQclear(res);
            throw std::runtime_error("Failed to retrieve value from result set.");
        }

        PQclear(res);
    } catch(const std::runtime_error& e) {
        if (std::string(e.what()) == "ROWS_NOT_FOUND") {
          // Possible fallback: ignore no rows 
        } else { throw; }// rethrow other runtime errors
    }
    catch (const std::exception& ex) {
        std::cerr << "[DbUtil::executeScalar] Exception: " << ex.what() << std::endl;
	throw std::runtime_error("DbUtil::executeScalar failed: " + std::string(ex.what()));
    }
    return rows;
}
/*
 * getRowCount is a function that returns no of records returned by query
 * if ROWS_NOT_FOUND, return -1
 */
int DbUtil::getRowCount(const std::string& query) {
    int rows=-1;
    try {
    auto conn = pool_->getConnection();
    PGresult* res = PQexec(conn->get(), query.c_str());
    if (!res) {
      throw std::runtime_error("Null PGresult returned from PQexec");
    }
    ExecStatusType status = PQresultStatus(res);
    if (status != PGRES_TUPLES_OK) {
    std::string errMsg = PQerrorMessage(conn->get());
    PQclear(res);
    throw std::runtime_error("Query did not return tuples. PostgreSQL error: " + errMsg);
    }
    rows = PQntuples(res);
    int cols = PQnfields(res);
    if (rows == 0) {
    PQclear(res);
    }
    PQclear(res);
    } catch(const std::runtime_error& e) {
      throw; 
    }
    catch (const std::exception& ex) {
    std::cerr << "[DbUtil::executeScalar] Exception: " << ex.what() << std::endl;
	  throw std::runtime_error("DbUtil::executeScalar failed: " + std::string(ex.what()));
    }
    return rows;
}

// Health check
bool DbUtil::isAlive() {
    try {
        std::string result = executeScalar("SELECT 1");
        return (result == "1");
    } catch (const std::exception& ex) {
        std::cerr << "[DbUtil::isAlive] Exception: " << ex.what() << std::endl;
        return false;
    }
}

void DbUtil::prepareStatement(const std::string& stmtName,const std::string& query,const int  paramsSize)
{
    try {
        auto conn = pool_->getConnection();
        // Prepare statement if not already prepared
        PGresult* prepRes = PQprepare(conn->get(), stmtName.c_str(), query.c_str(), paramsSize, nullptr);
        if (PQresultStatus(prepRes) != PGRES_COMMAND_OK) {
            std::string errMsg = PQerrorMessage(conn->get());
            PQclear(prepRes);
            throw std::runtime_error("PostgreSQL Statement Prepare failed: " + errMsg);
        }
        PQclear(prepRes);
    } catch (const std::exception& ex) {
        std::cerr << "[DbUtil::executePrepared] Exception: " << ex.what() << std::endl;
        throw;
    }
}

/* Function: exectuePrepared
 * take Statement Name, query, number of parameters in vector format 
 * Output comes in restulsTable
 */
 
void DbUtil::executePrepared(
    const std::string& stmtName,
    const std::string& query,
    const std::vector<std::string>& params,
    std::vector<std::vector<std::string>>& resultTable,
    int& affectedRows)
{
    affectedRows = 0;
    try {
        auto conn = pool_->getConnection();
        // Convert params to const char* array
        std::vector<const char*> paramValues;
        for (const auto& p : params) {
            paramValues.push_back(p.c_str());
        }
        std::cout << "Prepared Query: " << DbUtil::formatQuery(query.c_str(), params) << std::endl;
        // Execute prepared statement
        PGresult* execRes = PQexecPrepared(conn->get(), stmtName.c_str(), params.size(),
                                           paramValues.data(), nullptr, nullptr, 0);
        
        ExecStatusType status = PQresultStatus(execRes);
        if (status == PGRES_TUPLES_OK) {
            // SELECT query: fetch rows
            int rows = PQntuples(execRes);
            int cols = PQnfields(execRes);
            for (int i = 0; i < rows; ++i) {
                std::vector<std::string> row;
                for (int j = 0; j < cols; ++j) {
                    row.push_back(PQgetvalue(execRes, i, j));
                }
                resultTable.push_back(row);
            }
        } else if (status == PGRES_COMMAND_OK) {
            // INSERT/UPDATE/DELETE: get affected rows
            const char* rowCountStr = PQcmdTuples(execRes);
            if (rowCountStr && *rowCountStr) {
                affectedRows = std::atoi(rowCountStr);
            }
            std::cout << "[INFO] Command executed successfully. Rows affected: " << affectedRows << std::endl;
        } else {
            std::string errMsg = PQerrorMessage(conn->get());
            PQclear(execRes);
            throw std::runtime_error("Execution failed: " + errMsg);
        }

        PQclear(execRes);
    } catch (const std::exception& ex) {
        std::cerr << "[DbUtil::executePrepared] Exception: " << ex.what() << std::endl;
        throw;
    }
}

std::string DbUtil::formatQuery(const std::string& query, const std::vector<std::string>& params) {
    std::string formatted = query;
    for (size_t i = 0; i < params.size(); ++i) {
        std::string placeholder = "$" + std::to_string(i + 1);
        size_t pos = formatted.find(placeholder);
        if (pos != std::string::npos) {
            formatted.replace(pos, placeholder.length(), "'" + params[i] + "'");
        }
    }
    return formatted;
}

//g++ -std=c++11 -I/usr/pgsql-16/include -L/usr/pgsql-16/lib main.cpp DbUtil.cpp -o test -lpq -pthread
