#include <iostream>
#include "commons.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <cassert>

enum server_names {A, B, C, D, E, F, G, H};

void sendPhaseMessage(int *server_sockets) {
    /*
     *  Send a message to all the servers signaling the next phase
     *  and asking them to re-partition accordingly.
     */
    int phase_start = START_PHASE;
    for (int i=0; i<NUM_SERVERS; i++) {
        if (send(server_sockets[i], &phase_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending start phase message to server " << (char)('A'+i) << std::endl;
            close(server_sockets[i]);
            continue;
        }
        std::cout << "Sent start phase message to server " << (char)('A'+i) << std::endl;
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

    for (auto& thread: server_response_threads){
        thread.join();
    }
    std::cout << "Partitioning complete\n\n\n\n";
}

void sendUpdateToServer(int *server_sockets, int server_id)  {
    /*
     *  Send an UPDATE message to `server_id` and a NONE message to the remaining servers.
     */
    int* success_msg_buffer = new int;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i == server_id) {
            *success_msg_buffer = UPDATE;
            std::cout << "Sending an update on X to server" << (char) ('A'+server_id) << std::endl;
        }
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
    }

    //Receive Response from server
    int response_buf;
    if (recv(server_sockets[server_id], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else
        std::cout<< "Received response from server " << (char) ('A'+server_id) << '\n' << std::endl;
}

void sendTerminate(int* server_sockets) {
    /*
     *  Party's over. Wrap up.
     */
    for (int i = 0; i < NUM_SERVERS; i++) {
        int success_msg_buffer = END_PHASE;
        int bytes_rd = send(server_sockets[i],  &success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send phase terminate to server" << i << std::endl;
        else
            std::cout << "Sent phase terminate to server" << i << std::endl;
    }

    // Close sockets
    for (int i = 0; i < NUM_SERVERS; i++) {
        close(server_sockets[i]);
    }
}

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
    }

    /*************************************************************/
    std::cout << "\n\n\n\nBegin PHASE1" << std::endl;

    // Send "PHASE1" message to server
    sendPhaseMessage(server_sockets);
    sendUpdateToServer(server_sockets, A);
    sendUpdateToServer(server_sockets, B);

    /*************************************************************/
    std::cout << "\n\n\n\nBegin PHASE2" << std::endl;

    // Send "PHASE2" message to server
    sendPhaseMessage(server_sockets);
    sendUpdateToServer(server_sockets, A);
    sendUpdateToServer(server_sockets, B);
    sendUpdateToServer(server_sockets, E);
    sendUpdateToServer(server_sockets, F);

    /*************************************************************/
    std::cout << "\n\n\n\nBegin PHASE3" << std::endl;

    // Send "PHASE3" message to servers
    sendPhaseMessage(server_sockets);
    sendUpdateToServer(server_sockets, A);
    sendUpdateToServer(server_sockets, A);
    sendUpdateToServer(server_sockets, B);
    sendUpdateToServer(server_sockets, D);
    sendUpdateToServer(server_sockets, E);
    sendUpdateToServer(server_sockets, F);
    sendUpdateToServer(server_sockets, H);
    sendUpdateToServer(server_sockets, H);

    /*************************************************************/
    std::cout << "\n\n\n\nBegin PHASE4" << std::endl;

    // Send "PHASE4" message to server
    sendPhaseMessage(server_sockets);
    sendUpdateToServer(server_sockets, B);
    sendUpdateToServer(server_sockets, D);

    // Send Terminate message
    sendTerminate(server_sockets);

    return 0;
}