#pragma once

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>

#include "../utils/networking.h"

// @TODO figure out project structure to have this also be able to be built as
// standalone server from cmake

const int MAX_MESSAGE_SIZE = 36;

class Server {
public:
    Server() {}
    int init();
    int run_server();
    int make_server_sockaddr(struct sockaddr_in *addr, int port);
    void remove_pfds(int i);

    int fdSize = 3;
    struct pollfd pfds[3]; // 1 listener, 2 clients -- eventually more
    int fdCount = 0;
    int listenerfd;

    int PORT = 5488;
};