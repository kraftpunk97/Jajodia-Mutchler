#include <iostream>
#include "commons.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <cassert>

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

    // Controller now waits for confirmation from the servers...
    std::cout << "Waiting for confirmation from the servers\n";
    auto partitionResponseUtil = [](int server_socket_fd) {
        int* server_response_buffer = new int;
        int bytes_recv = 0;
        while(bytes_recv == 0) {
            bytes_recv = recv(server_socket_fd, (int *) server_response_buffer, sizeof(int), 0);
        }
        assert(*server_response_buffer == 1);
    };
    std::thread server_response_threads[NUM_SERVERS];

    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx] = std::thread(partitionResponseUtil, server_sockets[thread_idx]);
    }
    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx].join();
    }
    std::cout << "Partitioning complete\n";

    //Send update to server A
    int* success_msg_buffer = new int;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==0)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
    }
    
        
    // Close sockets
    for (int i = 0; i < NUM_SERVERS; i++) {
        close(server_sockets[i]);
    }

    return 0;
}
