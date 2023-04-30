#include "include/serversocket.hpp"
#include <iostream>

enum server_name {A, B, C, D, E, F, G, H};

enum controller_messages {NONE, START_PHASE, UPDATE, END_PHASE};

class Server {
public:
    ServerSocket m_controllerSocket;
    ServerSocket m_serverSocket;

    Server(int port): m_serverSocket(port) {
       m_serverSocket.accept(m_controllerSocket);
       std::cout << "Connected to controller" << std::endl;
    }

    void ControllerCommand(){
        int* controllerMsg = new int;
        int bytes_read = m_controllerSocket.recv(controllerMsg, sizeof(int));

        /*
         *  IF we get a None Start phase message, some "undefined-as-of-now" behaviour
         *  If we get a UPDATE MEssage,
         * */
        switch(*controllerMsg) {
            case NONE: std::cout << "Received None\n";
            break;
            case START_PHASE: std::cout << "Received START_PHASE\n";
            break;
            case UPDATE: std::cout << "Received UPDATE\n";
            break;
            case END_PHASE: std::cout << "Received END_PHASE\n";
            break;
        }
    }
};

int main(int argc, char** argv) {
    if (argc < 1) {
        std::cout << "Usage: <controller ip> <controller port>";
        return 1;
    }
    Server server(8088);

    server.ControllerCommand();
    return 0;
}