#include <iostream>
#include "commons.h"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

enum server_name {A, B, C, D, E, F, G, H};
int main() {
    // Create sockets and connect to servers
    int server_sockets[NUM_SERVERS];
    for (int i = 0; i < NUM_SERVERS; i++) {
        // Create socket
        server_sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sockets[i] < 0) {
            std::cerr << "Error creating socket for server " << i << std::endl;
            continue;
        }

        // Connect to server
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(servers[i].port);
        if (inet_pton(AF_INET, servers[i].ip.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Error converting server address for server " << i << std::endl;
            continue;
        }
        if (connect(server_sockets[i], (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            //std::cerr << "Error connecting to server " << i << std::endl;
            perror("connect: ");
            continue;
        }

        // Send "PHASE1" message to server
        int phase1_start = START_PHASE;
        if (send(server_sockets[i], &phase1_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending PHASE1 message to server " << (char)('A'+i) << std::endl;
            close(server_sockets[i]);
            continue;
        }

        std::cout << "Sent PHASE1 message to server " << (char)('A'+i) << std::endl;

    }

    // Close sockets
    for (int i = 0; i < NUM_SERVERS; i++) {
        close(server_sockets[i]);
    }

    return 0;
}
