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
    {"10.176.69.39", 8088},
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
            continue;
        }
        if (connect(server_sockets[i], (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Error connecting to server " << (char)('A'+i) << std::endl;
            continue;
        }

        // Send "PHASE1" message to server
        int phase1_start = 1;
        if (send(server_sockets[i], &phase1_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending PHASE1 message to server " << (char)('A'+i) << std::endl;
            close(server_sockets[i]);
            continue;
        }

        std::cout << "Sent PHASE1 message to server " << (char)('A'+i) << std::endl;

    }

    //SEND UPDATE
    int update_buf = 2;
    //Send update on X to Server A and B
    if ((send(server_sockets[0], &update_buf, sizeof(int), 0) < 0 ) || 
        ((send(server_sockets[1], &update_buf, sizeof(int), 0) < 0 ))) {
            std::cerr << "Error sending update message to server " << (char)('A') << std::endl;
    }
    else
    std::cout<< "Sent an update on X to server A and B"<< std::endl;

    //RECEIVE END OF PHASE 1
    int phase1_end_count;
    int phase1_end;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (recv(server_sockets[i], &phase1_end, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving phase 1 end from server " << (char)('A'+i) << std::endl;
            continue;
        }
        else {
            std::cout<< "Received end of Phase 1 msg from server "<< (char)('A'+i) << std::endl;
            phase1_end_count++;
        }
    }    
    if(phase1_end_count == NUM_SERVERS)
        std::cout<< "Received end of Phase 1 msg from all servers"<< std::endl;

    // Close sockets
    for (int i = 0; i < NUM_SERVERS; i++) {
        close(server_sockets[i]);
    }

    return 0;
}
