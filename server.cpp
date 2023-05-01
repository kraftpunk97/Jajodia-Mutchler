#include "include/serversocket.hpp"
#include "include/clientsocket.hpp"
#include "commons.h"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <regex>

// Hard-coding the phase partition, because I'm *this* close to losing it.
std::vector<std::string> phases {
    ":012:",
};
auto phase_itr = phases.begin();

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
        int* controllerMsg = new int;
        int bytes_read = m_controllerSocket.recv(controllerMsg, sizeof(int));

        /*
         *  IF we get a None Start phase message, some "undefined-as-of-now" behaviour
         *  If we get a UPDATE Message,
         * */
        
        switch(*controllerMsg) {
            case NONE: std::cout << "Received None\n";
            break;
            case START_PHASE: phase();
            break;
            case UPDATE: std::cout << "Received UPDATE\n";
            break;
            case END_PHASE: std::cout << "Received END_PHASE\n";
            break;
        }
    }

    void phase() {
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

        // ...And form new ties.
        while (it != end) {
            std::string partition = it->str();

            // Am I in this partition?
            auto designation = std::to_string(m_designation);
            if (partition.find(designation) != std::string::npos) {
                // Who are my neighbors?
                for (int peer: partition) {
                    peer -= '0';
                    if (peer != m_designation) {
                        m_peers.push_back(peer);
                    }
                }
                // Meeting the neighbors...
                // Two parts that have to be done simultaneously...
                // 1. Connect to other servers
                auto connToUtil = [=]() {
                    for (int peer: m_peers) {
                        std::string ip = servers[peer].ip;
                        int port = servers[peer].port;
                        m_peerToSockets[peer].connect(ip, port);
                    }
                };
                std::thread connToThread = std::thread(connToUtil);

                // 2. Listening for connections from other servers
                for (int i=0; i<m_peers.size(); i++) {
                    ServerSocket new_socket;
                    m_serverSocket.accept(new_socket);
                    m_peerFromSockets[i] = new_socket;
                }
                connToThread.join();
                break;
            }
            it++;
        }
        phase_itr++;

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
    return 0;
}