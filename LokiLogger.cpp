/*
   build this to test Loki_logger alone
   g++ -I../inc/ -D__TEST_LOKILOGGER__ -o lokilogger LokiLogger.cpp -lcurl
   Execute in bash shell : ./lokilogger
*/
#ifdef __TEST_LOKILOGGER__
#include "LokiLogger.h"
int main() {
   // LokiLogger logger("http://192.168.149.107:3100/loki/api/v1/push", "cpp-threaded");

    LokiLogger logger;

    logger.init("http://192.168.149.107:3100/loki/api/v1/push", "smsc-logger");

    // Use logger from multiple threads
    logger.log("Thread-safe log-fatal", 1);
    logger.log("Thread-safe log-error", 2);
    logger.log("Thread-safe log-warn", 3);
    logger.log("Thread-safe log-info", 4);
    logger.log("Thread-safe log-debug", 5);
    logger.log("Thread-safe log-debug-default");

    logger.fatal("Thread-safe log-fatal method");
    logger.error("Thread-safe log-error method");
    logger.warn("Thread-safe log-warn method");
    logger.info("Thread-safe log-info method");
    logger.debug("Thread-safe log-debug method");
        

    // At end of program:
    LokiLogger::shutdown();
    return 0;
}
#endif
