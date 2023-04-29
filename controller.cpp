#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NUM_SERVERS 8

// Struct to hold server information
typedef struct {
    char *ip;
    int port;
} server_info;

int main() {
    // Array of server information
    server_info servers[NUM_SERVERS] = {
    {"10.176.69.32", 8081},
    {"10.176.69.33", 8082},
    {"10.176.69.34", 8083},
    {"10.176.69.35", 8084},
    {"10.176.69.36", 8085},
    {"10.176.69.37", 8086},
    {"10.176.69.38", 8087},
};

    // Create sockets and connect to servers
    int server_sockets[NUM_SERVERS];
    for (int i = 0; i < NUM_SERVERS; i++) {
        // Create socket
        server_sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sockets[i] < 0) {
            std::cerr << "Error creating socket for server " << (char)('A'+i) << std::endl;
            continue;
        }

        // Connect to server
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(servers[i].port);
        if (inet_pton(AF_INET, servers[i].ip, &server_addr.sin_addr) <= 0) {
            std::cerr << "Error converting server address for server " << (char)('A'+i) << std::endl;
            close(server_sockets[i]);
            continue;
        }
        if (connect(server_sockets[i], (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Error connecting to server " << (char)('A'+i) << std::endl;
            close(server_sockets[i]);
            continue;
        }

        // Send "START" message to server
        const char* start_msg = "START";
        if (send(server_sockets[i], start_msg, strlen(start_msg), 0) < 0) {
            std::cerr << "Error sending START message to server " << (char)('A'+i) << std::endl;
            close(server_sockets[i]);
            continue;
        }

        std::cout << "Sent START message to server " << (char)('A'+i) << std::endl;
    }

    // Close sockets
    for (int i = 0; i < NUM_SERVERS; i++) {
        close(server_sockets[i]);
    }

    return 0;
}
