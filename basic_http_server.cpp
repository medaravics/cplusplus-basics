// Compile : g++ basic_http_server.cpp -o http_server
// Run : ./http_server
// curl http://localhost:8080
#include <iostream>
#include <unistd.h>       // for read(), write(), close()
#include <netinet/in.h>   // for sockaddr_in
#include <cstring>        // for memset(), strlen()
#include <cstdlib>        // for EXIT_FAILURE
#include <cstdio>         // for perror()

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    const char *http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, world!";

    // 1. Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind to port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // 4. Accept connection
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);  // Read HTTP request
        std::cout << "Received request:\n" << buffer << std::endl;

        send(new_socket, http_response, strlen(http_response), 0);  // Respond
        close(new_socket);
    }

    return 0;
}
