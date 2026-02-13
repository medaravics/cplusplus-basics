#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9090
#define BUFFER_SIZE 1024

class MySocketApp {
public:
    void runAsServer() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("Server socket creation failed");
            return;
        }

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("Bind failed");
            close(server_fd);
            return;
        }

        if (listen(server_fd, 1) < 0) {
            perror("Listen failed");
            close(server_fd);
            return;
        }

        std::cout << "[Server] Listening on port " << PORT << std::endl;

        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &len);
        if (client_fd < 0) {
            perror("Accept failed");
            close(server_fd);
            return;
        }

        char buffer[BUFFER_SIZE];
        int n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "[Server] Received: " << buffer << std::endl;

            const char* reply = "Hello from server";
            write(client_fd, reply, strlen(reply));
        }

        close(client_fd);
        close(server_fd);
    }

    void runAsClient(const char* server_ip) {
        sleep(1); // Optional: allow server to start

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Client socket creation failed");
            return;
        }

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr(server_ip);

        std::cout << "[Client] Connecting to " << server_ip << ":" << PORT << std::endl;

        if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connect failed");
            close(sock);
            return;
        }

        const char* message = "Hello from client";
        write(sock, message, strlen(message));

        char buffer[BUFFER_SIZE];
        int n = read(sock, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "[Client] Received: " << buffer << std::endl;
        }

        close(sock);
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:\n  " << argv[0] << " server\n  " << argv[0] << " client <server_ip>\n";
        return 1;
    }

    MySocketApp app;

    if (std::string(argv[1]) == "server") {
        app.runAsServer();
    } else if (std::string(argv[1]) == "client") {
        if (argc < 3) {
            std::cerr << "Please provide server IP.\n";
            return 1;
        }
        app.runAsClient(argv[2]);
    } else {
        std::cerr << "Invalid mode: " << argv[1] << "\nUse 'server' or 'client'\n";
        return 1;
    }

    return 0;
}
