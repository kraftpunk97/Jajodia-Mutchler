//
// Created by kxg220013 on 5/1/2023.
//

#ifndef JAJODIA_MUTCHLER_COMMONS_H
#define JAJODIA_MUTCHLER_COMMONS_H

#define NUM_SERVERS 3

enum controller_messages {NONE, START_PHASE, UPDATE, END_PHASE};

// Struct to hold server information
typedef struct {
    std::string ip;
    int port;
} server_info;

// Array of server information
server_info servers[NUM_SERVERS] = {
    {"10.176.69.32", 8080},
    {"10.176.69.33", 8080},
    {"10.176.69.34", 8080},
    /*{"10.176.69.35", 8084},
    {"10.176.69.36", 8085},
    {"10.176.69.37", 8086},
    {"10.176.69.38", 8087},
    {"10.176.69.39", 8088},*/
};

#endif //JAJODIA_MUTCHLER_COMMONS_H
