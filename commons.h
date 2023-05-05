//
// Created by kxg220013 on 5/1/2023.
//

#ifndef JAJODIA_MUTCHLER_COMMONS_H
#define JAJODIA_MUTCHLER_COMMONS_H

#include <iostream>
#include <vector>

#define NUM_SERVERS 8

enum controller_messages {NONE, START_PHASE, UPDATE, END_PHASE};

// Hard-coding the phase partition, because I'm *this* close to losing it.
// All phases should end with a `:`
std::vector<std::string> phases {
    "01234567:",
    "0123:4567:",
    "0:123:456:7:",
    "0:123456:7:"
};

// Struct to hold server information
typedef struct {
    std::string ip;
    int port;
} server_info;

//Struct to hold Object X
struct ObjectX {
    int VN;
    int RU;
    int DS;
    int server_id;
};


// Array of server information
server_info servers[NUM_SERVERS] = {
    {"172.30.172.145", 8080},
    {"172.30.172.145", 8081},
    {"172.30.172.145", 8082},
    {"172.30.172.145", 8083},
    {"172.30.172.145", 8084},
    {"172.30.172.145", 8085},
    {"172.30.172.145", 8086},
    {"172.30.172.145", 8087},
};

#endif //JAJODIA_MUTCHLER_COMMONS_H
