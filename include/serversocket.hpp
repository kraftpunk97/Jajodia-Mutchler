#ifndef SERVER_SOCKET_
#define SERVER_SOCKET_

#include "socket.hpp"
#include "socketexception.hpp"
#include <string>

class ServerSocket: private Socket {
public:
    ServerSocket(int port);
    ServerSocket();

    const ServerSocket& operator<< (const std::string& message) const; // For sending messages.
    const ServerSocket& operator>> (std::string& message); // For receiving messages.

    int recv(void * buffer, size_t len);
    void send(const void* buffer, size_t len);
    void accept(ServerSocket& new_socket) const;
    bool is_valid() {return Socket::is_valid(); };
    void close();

    void set_non_blocking(const bool blocking) { Socket::set_non_blocking(blocking); };
};
#endif