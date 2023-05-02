#include "include/serversocket.hpp"
#include "include/clientsocket.hpp"
#include "include/socketexception.hpp"
#include "commons.h"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <regex>
#include <unistd.h>
#include <poll.h>

#define VOTE 1
// Hard-coding the phase partition, because I'm *this* close to losing it.
std::vector<std::string> phases {
    "4:012:5",
};
auto phase_itr = phases.begin();

int VoteFlag = 0;

class Server {
public:
    int m_designation;
    ServerSocket m_controllerSocket;
    ServerSocket m_serverSocket;
    ServerSocket m_peerFromSockets[NUM_SERVERS];
    ClientSocket m_peerToSockets[NUM_SERVERS];
    std::vector<int> m_peers;


    Server(int port, int designation): m_serverSocket(port) {
        ServerSocket new_socket;
        m_designation = designation;
        m_serverSocket.accept(new_socket);
        std::cout << "Connected to controller" << std::endl;
        m_controllerSocket = new_socket;
    }

    void ControllerCommand(){
        int endphase_flag = 0;
        while (!endphase_flag) {
            int* controllerMsg = new int;
            int bytes_read = m_controllerSocket.recv(controllerMsg, sizeof(int));

            /*
            *  IF we get a None Start phase message, some "undefined-as-of-now" behaviour
            *  If we get a UPDATE Message,
            * */
            
            switch(*controllerMsg) {
                case NONE: {
                    std::cout << "Received None\n";
                    poll_VoteReq();
                }
                break;
                case START_PHASE: phase();
                break;
                case UPDATE: {
                    std::cout << "Received UPDATE on object X\n";
                    Send_VoteReq();
                }
                break;
                case END_PHASE: {
                    endphase_flag = 1;
                    std::cout << "Received END_PHASE\n";
                }

                break;
            }
        }
        
    }

    void phase() {
        std::cout << "Received \"PHASE\" message from the controller.\n";
        std::string current_phase = *phase_itr;
        std::regex pattern("\\d+");
        std::sregex_iterator it(current_phase.begin(), current_phase.end(), pattern);
        std::sregex_iterator end;

        // Cut all ties...
        for (ServerSocket peer_socket: m_peerFromSockets) {
            if (peer_socket.is_valid()) {
                peer_socket.close();
            }
        }
        for (ClientSocket peer_socket: m_peerToSockets) {
            if (peer_socket.is_valid()) {
                peer_socket.close();
            }
        }

        // ...And form new bonds. (very poetic)
        while (it != end) {
            std::string partition = it->str();

            // Am I in this partition?
            auto designation = std::to_string(m_designation);
            if (partition.find(designation) != std::string::npos) {
                std::cout << "Found the partition\n";
                // Who are my neighbors?
                for (int peer: partition) {
                    peer -= '0';
                    if (peer != m_designation) {
                        m_peers.push_back(peer);
                    }
                }
                std::cout << "My peers are:";
                for (int peer: m_peers)
                    std::cout << " " << peer;
                std::cout << std::endl;


                // Hi-Diddly-Ho, Neighborino!
                // Two parts that have to be done simultaneously...
                // 1. Listen for connections from other servers
                auto connFromUtil = [=]() {
                    for (int i = 0; i < m_peers.size(); i++) {
                        ServerSocket new_socket;
                        m_serverSocket.accept(new_socket);
                        std::cout << "Found a connection...\n";
                        m_peerFromSockets[i] = new_socket;
                    }
                };
                std::thread connFromThread = std::thread(connFromUtil);

                sleep(1);
                // 2. Connect to other servers
                auto connToUtil = [=]() {
                    for (int peer: m_peers) {
                        m_peerToSockets[peer] = ClientSocket();
                        std::string ip = servers[peer].ip;
                        int port = servers[peer].port;
                        std::cout << "IP: " << ip << "\tPort: " << port << std::endl;
                        m_peerToSockets[peer].connect(ip, port);
                        std::cout << "Connected to the server\n";
                    }
                };
                std::thread connToThread = std::thread(connToUtil);

                connToThread.join();
                connFromThread.join();
                break;
            }
            it++;
        }
        phase_itr++;

        sleep(1);
        // Inform the controller that the partitioning is complete.
        int* success_msg_buffer = new int;
        *success_msg_buffer = 1;
        m_controllerSocket.send(success_msg_buffer, sizeof(int));
        std::cout << "Sent the partitioning confirmation to the controller.\n";
    }

    void close() {
        /*
         * Because we're fucking decent human beings...
         * */
        sleep(1);
        for (ServerSocket peer_socket: m_peerFromSockets) {
            if (peer_socket.is_valid()) {
                try {
                    peer_socket.close();
                } catch (SocketException& e) {}
            }
        }
        for (ClientSocket peer_socket: m_peerToSockets) {
            if (peer_socket.is_valid()) {
                try {
                    peer_socket.close();
                } catch (SocketException& e) {}
            }
        }
        try {
            m_controllerSocket.close();
        } catch (SocketException& e) {}

        try {
            m_serverSocket.close();
        } catch (SocketException& e) {}
    }

    
    int CardSetP = m_peers.size();
    ObjectX SendInfo; //ADD DETAILS

    void poll_VoteReq() {
        struct pollfd pfds[CardSetP];

        for(int i = 0; i < CardSetP; i++) {
            pfds[i].fd = m_peerFromSockets[i].get_file_desc();
            pfds[i].events = POLLIN;
        }

        int ret =0;
        ret = poll(pfds, CardSetP, 10);
        if(ret) {
            for (int j = 0; j < CardSetP; j++) {
                if(pfds[j].revents & POLLIN) {
                    ObjectX* ListenToReqbuf = new ObjectX;
                    int bytes_read = m_peerFromSockets[j].recv((ObjectX*) ListenToReqbuf, sizeof(ObjectX));
                    if(bytes_read < 0)
                        std::cerr << "Error receiving Vote Request from server " << m_peers[j] << std::endl;
                    else {
                        
                        int send_socket_idx = ListenToReqbuf->server_id;
                        send(m_peerToSockets[send_socket_idx].get_file_desc(), &SendInfo, sizeof(ObjectX), 0);
                        std::cout << "Responded to Vote Request from server " << ListenToReqbuf->server_id << std::endl;
                    }   
                }
            }
        }
    }

    void Send_VoteReq() { 
        for (int i = 0; i < CardSetP; i++) {
            int peer_idx = m_peers[i];
            int bytes_read = send(m_peerToSockets[peer_idx].get_file_desc(), &SendInfo, sizeof(ObjectX), 0);
            if(bytes_read < 0)
                std::cerr << "Error sending Vote Request to server " << peer_idx << std::endl;
            else    
                std::cout << "Sent Vote Request to server " << peer_idx << std::endl;
        }
    }
};



int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Args: <server_designation> <server_port>\n";
        return 1;
    }

    int port, designation;
    port = std::atoi(argv[1]);
    designation = std::atoi(argv[2]);
    Server server(port, designation);
    server.ControllerCommand();
    server.close();
    return 0;
}