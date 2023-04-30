#ifndef SOCKET_EXCEPTION__
#define SOCKET_EXCEPTION__


#include <string>

class SocketException : public std::exception  {
public:
    SocketException(std::string message) : m_message(message) {}
    const char* what() {
        return m_message.c_str();
    }
private:
    std::string m_message;
};

#endif