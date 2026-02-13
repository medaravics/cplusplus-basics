#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <queue>
#include <map>
#include <sstream>

#define PORT 9090
#define BUFFER_SIZE 2048

class ClientHandler {
public:
    ClientHandler(int socket) : clientSocket(socket) {
        pthread_mutex_init(&queueMutex, NULL);
        pthread_cond_init(&queueCond, NULL);
    }

    ~ClientHandler() {
        close(clientSocket);
        pthread_mutex_destroy(&queueMutex);
        pthread_cond_destroy(&queueCond);
    }

    void start() {
        pthread_create(&receiverThreadId, NULL, &ClientHandler::receiverThreadFunc, this);
        pthread_create(&senderThreadId, NULL, &ClientHandler::senderThreadFunc, this);
        pthread_join(receiverThreadId, NULL);
        pthread_cancel(senderThreadId);
    }

private:
    int clientSocket;
    pthread_t receiverThreadId, senderThreadId;
    pthread_mutex_t queueMutex;
    pthread_cond_t queueCond;
    std::queue<std::pair<std::string, std::string> > sendQueue;

    static void* receiverThreadFunc(void *arg) {
        return static_cast<ClientHandler*>(arg)->receiverLoop();
    }

    static void* senderThreadFunc(void *arg) {
        return static_cast<ClientHandler*>(arg)->senderLoop();
    }

    void* receiverLoop() {
        char buffer[BUFFER_SIZE];
        while (true) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
            if (bytesRead <= 0) {
                std::cerr << "Client disconnected or read error." << std::endl;
                break;
            }
            std::string line(buffer);
            size_t sep = line.find('|');
            if (sep == std::string::npos) continue;
            std::string requestId = line.substr(0, sep);
            std::string data = line.substr(sep + 1);
            std::string response = processMessage(data);

            pthread_mutex_lock(&queueMutex);
            sendQueue.push(std::make_pair(requestId, response));
            pthread_mutex_unlock(&queueMutex);
            pthread_cond_signal(&queueCond);
        }
        return NULL;
    }

    void* senderLoop() {
        while (true) {
            pthread_mutex_lock(&queueMutex);
            while (sendQueue.empty()) {
                pthread_cond_wait(&queueCond, &queueMutex);
            }
            std::pair<std::string, std::string> message = sendQueue.front();
            sendQueue.pop();
            pthread_mutex_unlock(&queueMutex);

            std::string outMsg = message.first + "|" + message.second + "\n";
            ssize_t sent = write(clientSocket, outMsg.c_str(), outMsg.length());
            if (sent < 0) {
                perror("Send failed");
                break;
            }
        }
        return NULL;
    }

    std::string processMessage(const std::string &data) {
        std::string reversed(data.rbegin(), data.rend());
        return reversed;
    }
};

class Server {
public:
    void run() {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            perror("Socket creation failed");
            exit(1);
        }

        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(PORT);

        if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Bind failed");
            exit(1);
        }

        if (listen(serverSocket, 5) < 0) {
            perror("Listen failed");
            exit(1);
        }

        std::cout << "Server listening on port " << PORT << std::endl;

        while (true) {
            sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientLen);
            if (clientSocket < 0) {
                perror("Accept failed");
                continue;
            }

            pthread_t threadId;
            ClientHandler* handler = new ClientHandler(clientSocket);
            pthread_create(&threadId, NULL, [](void* arg) -> void* {
                ClientHandler* handler = static_cast<ClientHandler*>(arg);
                handler->start();
                delete handler;
                return NULL;
            }, handler);
            pthread_detach(threadId);
        }

        close(serverSocket);
    }
};

int main() {
    Server server;
    server.run();
    return 0;
}
