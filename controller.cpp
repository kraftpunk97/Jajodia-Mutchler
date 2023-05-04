#include <iostream>
#include "commons.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <cassert>
#include <strings.h>

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

    //Send update to server
    int* success_msg_buffer = new int;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==0)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server A
    int response_buf;
    if (recv(server_sockets[0], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server B
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==1)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server B
    if (recv(server_sockets[1], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

/*************************************************************/
    //Send "PHASE2" message to server
    int phase2_start = START_PHASE;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (send(server_sockets[i], &phase2_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending Start PHASE 2 message to server " << (char)('A'+i) << std::endl;
            continue;
        }
        std::cout << "Sent Start PHASE 2 message to server " << (char)('A'+i) << std::endl;
    }

    // Controller now waits for confirmation from the servers...
    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx] = std::thread(partitionResponseUtil, server_sockets[thread_idx]);
    }
    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx].join();
    }
    std::cout << "Partitioning complete\n";

    //Send update to server A
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==0)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server A
    if (recv(server_sockets[0], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server B
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==1)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server B
    if (recv(server_sockets[1], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server E
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==4)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server E
    if (recv(server_sockets[4], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server F
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==5)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server F
    if (recv(server_sockets[5], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server F " << std::endl;
    }
    else 
        std::cout<< "Received response from server F " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

/*************************************************************/
    // Send "PHASE3" message to server
    int phase3_start = START_PHASE;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (send(server_sockets[i], &phase3_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending Start PHASE 3 message to server " << (char)('A'+i) << std::endl;
            continue;
        }
        std::cout << "Sent Start PHASE 3 message to server " << (char)('A'+i) << std::endl;
    }

    //Controller now waits for confirmation from the servers...
    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx] = std::thread(partitionResponseUtil, server_sockets[thread_idx]);
    }
    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx].join();
    }
    std::cout << "Partitioning complete\n";

    //Send update to server A
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==0)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server A
    if (recv(server_sockets[0], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server A
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==0)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server A
    if (recv(server_sockets[0], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server B
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==1)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server B
    if (recv(server_sockets[1], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server D
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==3)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server D
    if (recv(server_sockets[3], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server D " << std::endl;
    }
    else 
        std::cout<< "Received response from server D " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

   //Send update to server E
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==4)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server E
    if (recv(server_sockets[4], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server F
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==5)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server F
    if (recv(server_sockets[5], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server F " << std::endl;
    }
    else 
        std::cout<< "Received response from server F " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

   //Send update to server H
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==7)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server H
    if (recv(server_sockets[7], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server H " << std::endl;
    }
    else 
        std::cout<< "Received response from server H " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server H
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==7)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server H
    if (recv(server_sockets[7], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server H " << std::endl;
    }
    else 
        std::cout<< "Received response from server H " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

/*************************************************************/
    // Send "PHASE4" message to server
    int phase4_start = START_PHASE;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (send(server_sockets[i], &phase4_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending Start PHASE 4 message to server " << (char)('A'+i) << std::endl;
            continue;
        }
        std::cout << "Sent Start PHASE 4 message to server " << (char)('A'+i) << std::endl;
    }

    // Controller now waits for confirmation from the servers...
    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx] = std::thread(partitionResponseUtil, server_sockets[thread_idx]);
    }
    for (int thread_idx=0; thread_idx<NUM_SERVERS; thread_idx++){
        server_response_threads[thread_idx].join();
    }
    std::cout << "Partitioning complete\n";

    //Send update to server B
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==1)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server B
    if (recv(server_sockets[1], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
    bzero(&response_buf, sizeof(response_buf));

    //Send update to server D
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (i==3)
            *success_msg_buffer = UPDATE;
        else
            *success_msg_buffer = NONE;
        int bytes_rd = send(server_sockets[i], (int *) success_msg_buffer, sizeof(int), 0);
        if(bytes_rd < 0)
            std::cerr << "Unable to send update on X to server" << i << std::endl;
        else
            std::cout << "Sent an update on X to server" << i << std::endl;
    }
    bzero(&success_msg_buffer, sizeof(success_msg_buffer));

    //Receive Response from Server D
    if (recv(server_sockets[3], &response_buf, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server D " << std::endl;
    }
    else 
        std::cout<< "Received response from server D " << std::endl;
    bzero(&response_buf, sizeof(response_buf));


    // Close sockets
    for (int i = 0; i < NUM_SERVERS; i++) {
        close(server_sockets[i]);
    }

    return 0;
}
