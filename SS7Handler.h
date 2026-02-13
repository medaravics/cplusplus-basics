#ifndef SS7HANDLER_H
#define SS7HANDLER_H

#include <queue>
#include <pthread.h>
#include <string>
#include "ServiceLogicRes.h"  // Assumes your struct definition is here

class SS7Handler {
public:
    SS7Handler();
    ~SS7Handler();

    bool start();
    void enqueueToSend(const ServiceLogicRes& msg);

private:
    static void* sendThreadFunc(void* arg);
    static void* receiveThreadFunc(void* arg);

    void processSendQueue();
    void processReceiveLoop();

    int cnxId;
    std::queue<ServiceLogicRes> sendQueue;
    pthread_mutex_t sendMutex;
    pthread_cond_t sendCond;

    pthread_t sendThread;
    pthread_t recvThread;
};

#endif