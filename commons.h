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
    {"10.176.69.32", 8080},
    {"10.176.69.33", 8081},
    {"10.176.69.34", 8082},
    {"10.176.69.35", 8083},
    {"10.176.69.36", 8084},
    {"10.176.69.37", 8085},
    {"10.176.69.38", 8086},
    {"10.176.69.39", 8087},
};

#endif //JAJODIA_MUTCHLER_COMMONS_H
