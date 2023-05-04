//
// Created by kxg220013 on 5/1/2023.
//

#ifndef JAJODIA_MUTCHLER_COMMONS_H
#define JAJODIA_MUTCHLER_COMMONS_H

#include <iostream>
#include <vector>

#define NUM_SERVERS 3

enum controller_messages {NONE, START_PHASE, UPDATE, END_PHASE};

// Hard-coding the phase partition, because I'm *this* close to losing it.
// All phases should end with a `:`
std::vector<std::string> phases {
    ":012::",
};

// Struct to hold server information
typedef struct {
    std::string ip;
    int port;
} server_info;

//Struct to hold Object X
typedef struct {
    int VN;
    int RU;
    int DS;
    int server_id;
} ObjectX;

// Array of server information
server_info servers[NUM_SERVERS] = {
    {"172.30.172.145", 8080},
    {"172.30.172.145", 8081},
    {"172.30.172.145", 8082},
    /*{"10.176.69.35", 8084},
    {"10.176.69.36", 8085},
    {"10.176.69.37", 8086},
    {"10.176.69.38", 8087},
    {"10.176.69.39", 8088},*/
};

#endif //JAJODIA_MUTCHLER_COMMONS_H
