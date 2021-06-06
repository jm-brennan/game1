#include "networking.h"

// @TODO this is a very bad float encoding/decoding scheme -- max float 32767, loss of precision
// use IEE-754 standard instead 
uint32_t encode_float(float f) {
    uint32_t encoded;
    uint32_t sign;
    if (f < 0) {
        sign = 1;
        f = -f;
    } else {
        sign = 0;
    }
    encoded = ((((uint32_t)f)&0x7fff)<<16) | (sign<<31);
    encoded |= (uint32_t)(((f - (int)f)*65536.0f))&0xffff;
    return encoded;
    
}

float decode_float(uint32_t ef) {
    float f = ((ef>>16)&0x7fff);
    f += (ef&0xffff) / 65536.0f;
    if (((ef>>31)&0x1) == 0x1) {
        f = -f;
    }
    return f;
}

int get_port_number(int sockfd) {
 	struct sockaddr_in addr;
	socklen_t length = sizeof(addr);
	if (getsockname(sockfd, (sockaddr *) &addr, &length) == -1) {
		perror("Error getting port of socket");
		return -1;
	}
	// Use ntohs to convert from network byte order to host byte order.
	return ntohs(addr.sin_port);
 }

int sendall(int socket, char* buf, int msgSize) {
    int totalSent = 0;
    int sent = 0;
    while (totalSent < msgSize) {
        sent = send(socket, buf, msgSize-totalSent, 0);
        if (sent == -1) break;
        totalSent += sent;
    }
    if (totalSent == msgSize) {
        return totalSent;
    } else {
        return -1;
    }
}

void make_local_server() {
    Server s;
    s.init();
    s.run_server();
}