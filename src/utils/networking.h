#pragma once
#include <arpa/inet.h>
#include <stdio.h>
#include "server/Server.h"

uint32_t encode_float(float f);
float decode_float(uint32_t ef);
int get_port_number(int sockfd);
int sendall(int socket, char* buf, int msgSize);
void make_local_server();
