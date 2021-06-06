#include "Client.h"

void Client::init() {
    tcpSockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    if (make_client_sockaddr(&addr) == -1) {
        perror("Error::Client()::Could not make socket\n");
    }
   
    if (connect(tcpSockfd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        printf("Could not find server, self hosting\n");
        std::thread t(make_local_server);
        t.detach();
        // @TODO this whole self hosting thing is gonna need a lot of work,
        // to start it would be good to use a conditional variable instad of
        // just the sleep to allow the server to get set up
        std::this_thread::sleep_for(std::chrono::microseconds(20000));
        if (connect(tcpSockfd, (sockaddr*)&addr, sizeof(addr)) == -1) {
            printf("ERRRO::Failed to connect to self hosted server\n");
        }
    }
    if (fcntl(tcpSockfd, F_SETFL, O_NONBLOCK) == -1) {
        perror("Error::Client()::setting to nonblocking");
    }
}

int Client::make_client_sockaddr(struct sockaddr_in *addr) {
	// specify socket family. This is an internet socket
	addr->sin_family = AF_INET;

	// specify socket address (hostname).
	// The socket will be a client, so call this unix helper function
	// to convert a hostname string to a useable `hostent` struct.
	struct hostent *host = gethostbyname(serverHostname.c_str());
	if (host == nullptr) {
		fprintf(stderr, "%s: unknown host\n", serverHostname.c_str());
		return -1;
	}
	memcpy(&(addr->sin_addr), host->h_addr, host->h_length);

	// Set the port value.
	// Use htons to convert from local byte order to network byte order.
	addr->sin_port = htons(PORT);

	return 0;
}

/* 
@TODO need general send that sends different types of data based on protocol
with headers and such so that the server and other client will know how to receive.
Also player state will need to be sent over UDP
 */
int Client::tcp_message_player_state(float* data, int size) {
    uint32_t encoded[9];
    for (int i = 0; i < size; i++) {
        encoded[i] = encode_float(data[i]);
    }

    // @TODO make and use sendall
    size_t numBytesToSend = sizeof(encoded);
    if (sendall(tcpSockfd, (char*)encoded, sizeof(encoded)) == -1) {
        perror("Error sending on stream socket");
        return -1;
	}
    return 0;
}

/* 
@TODO need general listener than can adapt what (and how much) data it will try to
receive and store based on a header int
 */
void Client::tcp_listen_player_state() {
    char msg[MAX_MESSAGE_SIZE];
    memset(msg, 0, sizeof(msg));
    // @TODO this receive needs to be more general to not just rely on 
    // getting the 9 floats, needs to be done with headers and variable sized bufs
    int numBytes = recv(tcpSockfd, msg, sizeof(msg), 0);
    if (numBytes == -1) return;
    else if (numBytes != MAX_MESSAGE_SIZE) printf("num bytes received is wrong\n");

    for (int i = 0; i < 9; i++) {
        playerData[i] = decode_float(*(uint32_t*)(msg + (i*sizeof(uint32_t))));
    }
    hasUpdate = true;
}

void Client::shutdown() {
    close(tcpSockfd);
}