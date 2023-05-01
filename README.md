# Jajodia-Mutchler

## How to run

### Controller

`g++ --std=c++11 -pthread controller.cpp -o control`

### Servers
`g++ -pthread --std=c++11 include/serversocket.cpp include/clientsocket.cpp include/socket.cpp server.cpp -o server
`