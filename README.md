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


#### What goes in the `commons.h` file? 
You must put in the external IP address in the `commons.h` file. You can use `ifconfig` to figure that out. When I run `ifconfig` on one of my servers, I get a response similar to what is shown below...

```
kgupta@ecelap261053 → ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet <***.***.***.***>  netmask 255.255.240.0  broadcast <***.**.***.***>
        inet6 <****::****:****:****:****>  prefixlen 64  scopeid 0x20<link>    
        ether <**:**:**:**:**:**>  txqueuelen 1000  (Ethernet)
        RX packets 17  bytes 2613 (2.6 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 10  bytes 796 (796.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

```

That means that I use the `inet` value of `eth0` for the server IP address, and any free port for the port number.

<sub><sup>If you take this course in the future and you get this exact same assignment, then I gotchu covered fam.</sup></sub>
