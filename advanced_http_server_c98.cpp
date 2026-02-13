// Compile : g++ advanced_http_server_c98.cpp -o http_server_v3 -lpthread
// Run : ./http_server
// curl http://localhost:8080
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/socket.h>

#define PORT 8080
#define TIMEOUT_SEC 5

void* handleClient(void* arg) {
    int client_socket = *(int*)arg;
    delete (int*)arg;

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    char buffer[1024] = {0};
    int bytesRead = read(client_socket, buffer, sizeof(buffer));

    if (bytesRead <= 0) {
        std::string timeout_response =
            "HTTP/1.1 408 Request Timeout\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 15\r\n"
            "\r\n"
            "Request Timeout";
        send(client_socket, timeout_response.c_str(), timeout_response.length(), 0);
        close(client_socket);
        return NULL;
    }

    std::string request(buffer);
    std::string response;

    if (request.substr(0, 3) != "GET") {
        response =
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "Bad Request";
    } else if (request.find("GET / ") != std::string::npos) {
        std::string msg = "Welcome C++98 Server";
        response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(msg.size()) + "\r\n"
            "\r\n" + msg;
    } else {
        std::string msg = "Not Found";
        response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(msg.size()) + "\r\n"
            "\r\n" + msg;
    }

    send(client_socket, response.c_str(), response.length(), 0);
    close(client_socket);
    return NULL;
}

int main() {
    int server_fd, client_socket;
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

    std::cout << "C++98 HTTP Server listening on port " << PORT << "...\n";

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        int* pclient = new int;
        *pclient = client_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handleClient, pclient);
        pthread_detach(tid);  // Auto-cleanup thread resources
    }

    close(server_fd);
    return 0;
}
