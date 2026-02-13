// Compile : g++ http_server_c98_v4.cpp -o http_server_v4 -lpthread
// Run : ./http_server4
// curl http://localhost:8080

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <cstdio>         // for perror()

#define PORT 8080
#define TIMEOUT_SEC 5

volatile bool stop_server = false;
int server_fd = -1;

void signalHandler(int signum) {
    std::cout << "\nReceived SIGINT. Shutting down server gracefully...\n";
    stop_server = true;
    if (server_fd != -1) {
        close(server_fd);
    }
}

std::string toString(int n) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", n);
    return std::string(buf);
}

void* handleClient(void* arg) {
    int client_socket = *(int*)arg;
    delete (int*)arg;

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    char buffer[1024] = {0};
    int bytesRead = read(client_socket, buffer, sizeof(buffer));

    std::string response;
    if (bytesRead <= 0) {
        std::string msg = "Request Timeout";
        response = "HTTP/1.1 408 Request Timeout\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " + toString(msg.size()) + "\r\n\r\n" + msg;
        send(client_socket, response.c_str(), response.length(), 0);
        close(client_socket);
        return NULL;
    }

    std::string request(buffer);
    if (request.substr(0, 3) != "GET") {
        std::string msg = "Bad Request";
        response = "HTTP/1.1 400 Bad Request\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " + toString(msg.size()) + "\r\n\r\n" + msg;
    } else if (request.find("GET / ") != std::string::npos) {
        std::string msg = "Welcome to C++98 HTTP Server";
        response = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " + toString(msg.size()) + "\r\n\r\n" + msg;
    } else {
        std::string msg = "Not Found";
        response = "HTTP/1.1 404 Not Found\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " + toString(msg.size()) + "\r\n\r\n" + msg;
    }

    send(client_socket, response.c_str(), response.length(), 0);
    close(client_socket);
    return NULL;
}

int main() {
    // Register signal handler
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = signalHandler;
    sigaction(SIGINT, &action, NULL);

    int client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "C++98 HTTP Server running on port " << PORT << "\n";
    std::cout << "Press Ctrl+C to stop the server\n";

    while (!stop_server) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            if (stop_server) break;  // expected during shutdown
            perror("Accept failed");
            continue;
        }

        int* pclient = new int;
        *pclient = client_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handleClient, pclient);
        pthread_detach(tid);
    }

    std::cout << "Server shutdown complete.\n";
    return 0;
}
