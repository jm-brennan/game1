#pragma once

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string>

#include <thread>
#include "utils/networking.h"

// @TODO @TEMP
const std::string serverHostname = "127.0.0.1";

class Client {
public:
    Client() {}
    void init();
    int make_client_sockaddr(struct sockaddr_in *addr);
    int tcp_message_player_state(float* data, int size);
    void tcp_listen_player_state();
    void shutdown();

    bool hasUpdate = false;
    float playerData[9];
private:
    const int MAX_MESSAGE_SIZE = 36;
    const int PORT = 5488;
    
    int tcpSockfd;
    //int updSockfd;
};
