#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NUM_SERVERS 8
#define START_PHASE 1
#define UPDATE 2
#define END_PHASE 3

// Struct to hold server information
typedef struct {
    char *ip;
    int port;
} server_info;

enum server_name {A, B, C, D, E, F, G, H};
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
        sockaddr_in server_addr;
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
        int phase1_start = START_PHASE;
        if (send(server_sockets[i], &phase1_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending PHASE1 message to server " << (char)('A'+i) << std::endl;
            close(server_sockets[i]);
            continue;
        }

        std::cout << "Sent PHASE1 message to server " << (char)('A'+i) << std::endl;

    }

    //Send update on X to Server A   
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 0))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server A"<< std::endl;

    //Receive "Response" from Server A
    int response_buf_A;
    if (recv(server_sockets[0], &response_buf_A, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;

    //Send update on X to Server B    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 1))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server B"<< std::endl;

    //Receive "response" from Server B
    int response_buf_B;
    if (recv(server_sockets[1], &response_buf_B, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
   
/*************************************************************/
    // Send "PHASE2" message to server
    int phase2_start = START_PHASE;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (send(server_sockets[i], &phase2_start, sizeof(int), 0) < 0) {
            std::cerr << "Error sending Start PHASE 2 message to server " << (char)('A'+i) << std::endl;
            continue;
        }
        std::cout << "Sent Start PHASE 2 message to server " << (char)('A'+i) << std::endl;
    }
    
    //Send update on X to Server A   
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 0))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server A"<< std::endl;

    //Receive "Response" from Server A
    int response_buf_A;
    if (recv(server_sockets[0], &response_buf_A, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;

    //Send update on X to Server B    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 1))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server B"<< std::endl;

    //Receive "response" from Server B
    int response_buf_B;
    if (recv(server_sockets[1], &response_buf_B, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
    
    //Send update on X to Server E   
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 4))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server E"<< std::endl;

    //Receive "Response" from Server E
    int response_buf_E;
    if (recv(server_sockets[4], &response_buf_E, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server E " << std::endl;
    }
    else 
        std::cout<< "Received response from server E " << std::endl;

    //Send update on X to Server F    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 5))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server F"<< std::endl;

    //Receive "response" from Server F
    int response_buf_F;
    if (recv(server_sockets[5], &response_buf_F, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server F " << std::endl;
    }
    else 
        std::cout<< "Received response from server F " << std::endl;

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
    
    //Send update on X to Server A   
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 0))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server A"<< std::endl;

    //Receive "Response" from Server A
    int response_buf_A;
    if (recv(server_sockets[0], &response_buf_A, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server A " << std::endl;
    }
    else 
        std::cout<< "Received response from server A " << std::endl;

    //Send update on X to Server B    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 1))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server B"<< std::endl;

    //Receive "response" from Server B
    int response_buf_B;
    if (recv(server_sockets[1], &response_buf_B, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
    
    //Send update on X to Server D    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 3))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server D"<< std::endl;

    //Receive "response" from Server D
    int response_buf_D;
    if (recv(server_sockets[3], &response_buf_D, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server D " << std::endl;
    }
    else 
        std::cout<< "Received response from server D " << std::endl;

    //Send update on X to Server E   
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 4))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server E"<< std::endl;

    //Receive "Response" from Server E
    int response_buf_E;
    if (recv(server_sockets[4], &response_buf_E, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server E " << std::endl;
    }
    else 
        std::cout<< "Received response from server E " << std::endl;

    //Send update on X to Server F    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 5))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server F"<< std::endl;

    //Receive "response" from Server F
    int response_buf_F;
    if (recv(server_sockets[5], &response_buf_F, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server F " << std::endl;
    }
    else 
        std::cout<< "Received response from server F " << std::endl;

    //Send update on X to Server H    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 7))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server H"<< std::endl;

    //Receive "response" from Server H
    int response_buf_H;
    if (recv(server_sockets[7], &response_buf_H, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server H " << std::endl;
    }
    else 
        std::cout<< "Received response from server H " << std::endl;

    //Send END OF PHASE 3
    int phase3_end = END_PHASE;
    for (int i = 0; i < NUM_SERVERS; i++) {
        if (send(server_sockets[i], &phase3_end, sizeof(int), 0) < 0) {
            std::cerr << "Error sending phase 3 end from server " << (char)('A'+i) << std::endl;
            continue;
        }
        else 
            std::cout<< "Sent end of Phase 3 msg to server "<< (char)('A'+i) << std::endl;      
    }
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

    //Send update on X to Server B    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 1))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server B"<< std::endl;

    //Receive "response" from Server B
    int response_buf_B;
    if (recv(server_sockets[1], &response_buf_B, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server B " << std::endl;
    }
    else 
        std::cout<< "Received response from server B " << std::endl;
    
    //Send update on X to Server D    
    for (int i = 0; i < NUM_SERVERS; i++) {
        int update_buf;
        if ((i == 3))
            update_buf = UPDATE;
        else 
            update_buf = 0;
        if(send(server_sockets[i], &update_buf, sizeof(int), 0) < 0 ) {
            std::cerr << "Error sending update message to server " << (char)('A'+i) << std::endl;
        }
    }
    std::cout<< "Sent an update on X to server D"<< std::endl;

    //Receive "response" from Server D
    int response_buf_D;
    if (recv(server_sockets[3], &response_buf_D, sizeof(int), 0) < 0) {
            std::cerr << "Error receiving response from server D " << std::endl;
    }
    else 
        std::cout<< "Received response from server D " << std::endl;
/*************************************************************/

    // Close sockets
    for (auto server_socket: server_sockets) {
        close(server_socket);
    }

    return 0;
}
