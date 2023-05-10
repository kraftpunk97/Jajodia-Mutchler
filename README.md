# Jajodia-Mutchler
The final project for the "Advanced Operating Systems" course (CS6378) (taught by Ravi Prakash), in which we implement the Jajodia-Mutchler algorithm.

## How to compile

### Controller

> g++ --std=c++11 -pthread controller.cpp -o control


### Servers
> g++ -pthread --std=c++11 include/serversocket.cpp include/clientsocket.cpp include/socket.cpp server.cpp -o server

## How to run
Activate the servers before the controller.

### Servers
`server_designation` is an integer between `[0, 7]`. Update the ip address and ports in the [`servers` object in the `commons.h` file](https://github.com/kraftpunk97/Jajodia-Mutchler/blob/2038f1bb7f3df511e95f41ee63437fd761de14d5/commons.h#L40).
> ./server <server_port> <server_designation>


### Controller

> ./control


<sub><sup>If you take this course in the future and you get this exact same assignment, then I gotchu covered fam.</sup></sub>
