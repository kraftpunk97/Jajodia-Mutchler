#include "socket.hpp"
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <fcntl.h>

Socket::Socket() {
    m_file_desc = -1;
    memset(&m_address, 0, sizeof(m_address));
}

bool Socket::close() {
    if(is_valid() and ::close(m_file_desc) == -1) {
        //perror("Error occured while closing socket: ");
        return false;
    }
    return true;
}

bool Socket::is_valid() const {return m_file_desc != -1;}


/* Server Initialization functions */

bool Socket::create() {
    /* Sets the file descriptor of the socket and sets the options. */
    m_file_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (!is_valid())
        return false;

    /* TODO: Let's play with it later...*/
    int on = 1;
    if (setsockopt(m_file_desc, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) < 0) {
        //perror("Error occured while creating socket: ");
        return false;
    }

    return true;
}


bool Socket::bind(const int port) {
    /* Configure the m_address to bind a socket to a port*/
    if (!is_valid())
        return false;

    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(port);
    if (::bind(m_file_desc, (const sockaddr*) &m_address, sizeof(m_address)) < 0) {
        //perror("Error occured while binding the socket to the port: ");
        return false;
    }
    
    return true;
}

bool Socket::listen(int backlog) const {
    /* If the socket is valid, then listen for incoming connections. */
    if (!is_valid())
        return false;
    
    if (::listen(m_file_desc, backlog) < 0) {
        //perror("Error occured while listening for active connections: ");
        return false;
    }    
    return true;
}

bool Socket::accept(Socket& new_socket) const {
    /* Accept the incoming connection. */
    int addr_len = sizeof(m_address);
    new_socket.m_file_desc = ::accept(m_file_desc, (sockaddr *) &m_address, (socklen_t *) &addr_len);
    if (new_socket.m_file_desc < 0) {
        //perror("Error occured while accepting incoming connection: ");
        return false;
    }
    return true;
}

/* Client Initialization functions */

bool  Socket::connect(const std::string host, const int port) {
    /* Connect to the server at `host` and `port`. */
    if (!is_valid())
        return false;

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (::inet_pton(AF_INET, host.c_str(), &server_address.sin_addr) == -1)
        return false;
    int connect_val = ::connect(m_file_desc, (sockaddr *) &server_address, sizeof(server_address));
    if (connect_val < 0) {
        //perror("Error occured while connecting: ");
        return false;
    }
    return true;
}

/* Data Transmission functions */

bool Socket::send(const std::string data) const {
    /* Send the `data` to established connection. */
    int bytes_sent = ::send(m_file_desc, data.c_str(), data.size(), MSG_NOSIGNAL);
    if(bytes_sent == -1) {
        //perror("Error occured while transmission of data: ");
    }
    return bytes_sent != -1;
}

bool Socket::send(const void* buffer, size_t len) const {
    int bytes_sent = ::send(m_file_desc, buffer, len, MSG_NOSIGNAL);
    if (bytes_sent == -1) {
        //perror("Error occured while transmission of data");
    }
    return bytes_sent != -1;
}

int Socket::recv(std::string& data) {
    /* Recieve the data from the sender. */
    char read_buffer[MAXREADSIZE+1];
    data = "";
    int bytes_read = ::recv(m_file_desc, read_buffer, MAXREADSIZE, 0);
    if(bytes_read == -1) {
        //perror("Error occured while receiving data: ");
    }
    data = read_buffer;
    return bytes_read;
}

int Socket::recv(void * buffer, size_t len) {
    int bytes_read = ::recv(m_file_desc, buffer, len, 0);
    if (bytes_read == -1) {
        //perror("Error occured while receiving data");
    }
    return bytes_read;
}

void Socket::set_non_blocking(const bool blocking) {
    int opts;
    opts = fcntl(m_file_desc, F_GETFL);

    if (opts < 0) {
        return;
    }

    opts = blocking ? (opts | O_NONBLOCK) : (opts & ~O_NONBLOCK);

    fcntl(m_file_desc, F_SETFL, opts);
}