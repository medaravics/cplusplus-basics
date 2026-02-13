#include "SS7Handler.h"
#include "Logger.h"     // for g_NgLogger.log()
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "TCAPStack.h"  // include your SS7 stack API declarations

#define MAX_FD 100

SS7Handler::SS7Handler() {
    pthread_mutex_init(&sendMutex, NULL);
    pthread_cond_init(&sendCond, NULL);
}

SS7Handler::~SS7Handler() {
    pthread_mutex_destroy(&sendMutex);
    pthread_cond_destroy(&sendCond);
}

bool SS7Handler::start() {
    cnxId = openTCAPConnection();
    if (cnxId < 0) {
        g_NgLogger.log(LOG_LEVEL_CRITICAL, "Failed to open SS7 TCAP connection");
        return false;
    }

    // Start sender and receiver threads
    pthread_create(&sendThread, NULL, &SS7Handler::sendThreadFunc, this);
    pthread_create(&recvThread, NULL, &SS7Handler::receiveThreadFunc, this);
    return true;
}

void SS7Handler::enqueueToSend(const ServiceLogicRes& msg) {
    pthread_mutex_lock(&sendMutex);
    sendQueue.push(msg);
    pthread_cond_signal(&sendCond);
    pthread_mutex_unlock(&sendMutex);
}

void* SS7Handler::sendThreadFunc(void* arg) {
    static_cast<SS7Handler*>(arg)->processSendQueue();
    return nullptr;
}

void* SS7Handler::receiveThreadFunc(void* arg) {
    static_cast<SS7Handler*>(arg)->processReceiveLoop();
    return nullptr;
}

void SS7Handler::processSendQueue() {
    while (true) {
        pthread_mutex_lock(&sendMutex);
        while (sendQueue.empty()) {
            pthread_cond_wait(&sendCond, &sendMutex);
        }

        ServiceLogicRes msg = sendQueue.front();
        sendQueue.pop();
        pthread_mutex_unlock(&sendMutex);

        // Implement TCX_send using msg.rest_data, etc.
        int sendResult = TCX_send(cnxId, msg.rest_data, strlen(msg.rest_data));
        if (sendResult < 0) {
            g_NgLogger.log(LOG_LEVEL_ERROR, "Failed to send message via SS7");
        } else {
            g_NgLogger.log(LOG_LEVEL_DEBUG, "Message sent to SS7: %s", msg.rest_data);
        }
    }
}

void SS7Handler::processReceiveLoop() {
    tcx_primitive L_tcxPrimitive;
    int L_more = -1, L_result = -1;
    int L_cnx_active[MAX_FD];
    int L_num_cxn = 0;
    int L_moreMsgs = 0;
    struct timeval L_timeVal;
    time_t counter_T;

    L_timeVal.tv_sec = 20;
    L_timeVal.tv_usec = 0;

    time(&counter_T);
    memset(L_cnx_active, 0, sizeof(L_cnx_active));

    while (true) {
        L_result = TCX_recv(cnxId, NULL, &L_tcxPrimitive, NULL, &L_more);
        if (L_result == 0) {
            g_NgLogger.log(LOG_LEVEL_DEBUG, "Received message from SS7");

            // Parse & wrap into ServiceLogicRes if needed
            ServiceLogicRes incoming;
            memset(&incoming, 0, sizeof(incoming));
            strncpy(incoming.rest_data, (char*)L_tcxPrimitive.data, sizeof(incoming.rest_data) - 1);

            // You can enqueue to a response queue or handle directly
            // Example:
            // responseQueue.push(incoming);
        } else {
            g_NgLogger.log(LOG_LEVEL_ERROR, "Error receiving SS7 message");
            sleep(1);  // avoid busy loop on error
        }
    }
}
