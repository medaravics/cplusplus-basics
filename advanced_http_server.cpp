// Compile : g++ advanced_http_server.cpp -o http_server_v2
// Run : ./http_server
// curl http://localhost:8080
#include <thread>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>       // for read(), write(), close()
#include <netinet/in.h>   // for sockaddr_in
#include <cstring>        // for memset(), strlen()
#include <cstdlib>        // for EXIT_FAILURE
#include <cstdio>         // for perror()

#define PORT 8080
#define TIMEOUT_SEC 5

std::string httpResponse(int status, const std::string& message) {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << status << " " << message << "\r\n"
        << "Content-Type: text/plain\r\n"
        << "Content-Length: " << message.size() << "\r\n"
        << "\r\n"
        << message;
    return oss.str();
}

void handleClient(int client_socket) {
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    char buffer[1024] = {0};
    int bytesRead = read(client_socket, buffer, sizeof(buffer));

    if (bytesRead < 0) {
        std::cerr << "Request timed out or failed.\n";
        std::string response = httpResponse(408, "Request Timeout");
        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
        return;
    }

    std::istringstream request(buffer);
    std::string method, path, version;
    request >> method >> path >> version;

    if (method != "GET" || version.substr(0, 5) != "HTTP/") {
        std::string response = httpResponse(400, "Bad Request");
        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
        return;
    }

    std::cout << "Received: " << method << " " << path << " " << version << "\n";

    if (path == "/") {
        std::string response = httpResponse(200, "Welcome to C++ HTTP Server");
        send(client_socket, response.c_str(), response.size(), 0);
    } else {
        std::string response = httpResponse(404, "Not Found");
        send(client_socket, response.c_str(), response.size(), 0);
    }

    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "HTTP Server running on port " << PORT << "\n";

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        std::thread t(handleClient, new_socket);
        t.detach();
    }

    close(server_fd);
    return 0;
}
