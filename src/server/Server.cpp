#include "Server.h"

void Server::remove_pfds(int i) {
    // if fd to remove is at end, just decrement count, otherwise
    // swap last fd with i and then decrement
    if (i != fdCount - 1) {
        pfds[i] = pfds[fdCount-1];
    }
    fdCount--;
}

int Server::make_server_sockaddr(struct sockaddr_in *addr, int port) {
	// This is an internet socket.
	addr->sin_family = AF_INET;

	// The socket will be a server, so it will only be listening.
	// Let the OS map it to the correct address.
	addr->sin_addr.s_addr = INADDR_ANY;

	// If port is 0, the OS will choose the port for us.
	// Use htons to convert from local byte order to network byte order.
	addr->sin_port = htons(port);

	return 0;
}

int Server::init() {
	{ // set up listening socket
        listenerfd = socket(AF_INET, SOCK_STREAM, 0);
        if (listenerfd == -1) {
            perror("Error opening stream socket");
            return -1;
        }

        int yesval = 1;
        if (setsockopt(listenerfd, SOL_SOCKET, SO_REUSEADDR, &yesval, sizeof(yesval)) == -1) {
            perror("Error setting socket options");
            return -1;
        }

        struct sockaddr_in addr;
        if (make_server_sockaddr(&addr, PORT) == -1) {
            return -1;
        }
        if (bind(listenerfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
            perror("Error binding stream socket");
            return -1;
        }

        PORT = get_port_number(listenerfd);
        printf("Server listening on port %d...\n", PORT);

        int queueSize = 10; // no real idea how this is coming into play
        listen(listenerfd, queueSize);
    }

    // set first poll data struct to be the listener
    pfds[0].fd = listenerfd;
    pfds[0].events = POLLIN; // ready to read incoming connection
    fdCount = 1;
    return 0;
}

int Server::run_server() {
    char msg[MAX_MESSAGE_SIZE];
	while (1) {
        memset(msg, 0, sizeof(msg));

        // @TODO could actually use the count to exit early in following loop
        int pollCount = poll(pfds, fdCount, -1);
        if (pollCount == -1) {
            perror("polling");
            exit(1);
        }

        for (int i = 0; i < fdCount; i++) {
            if (pfds[i].revents & POLLIN) { // fd i has data to read
                if (pfds[i].fd == listenerfd) { // listener, a client is asking for new connection
                    int newfd = accept(listenerfd, 0, 0);
                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        if (fdCount < fdSize) {
                            pfds[fdCount].fd = newfd;
                            pfds[fdCount].events = POLLIN;
                            fdCount++;
                            printf("New client FD: %d\n", newfd);
                        }
                    }
                } else { // message from a connected client
                    // @TODO this receive needs to be more general to not just rely on 
                    // getting the 9 floats, needs to be done with headers and variable sized bufs
                    int numBytes = recv(pfds[i].fd, msg, sizeof(msg), 0);
                    int sentFrom = pfds[i].fd;
                    if (numBytes <= 0) { // recv error
                        if (numBytes == 0) printf("Client %d disconnected\n", sentFrom);
                        else perror("recv");
                        close(sentFrom);
                        remove_pfds(i);
                    } else { // got real data
                        // @TODO eventual real work of server will go here for actually managing
                        // game state, for now just share all data automatically with all other clients
                        for (int j = 1; j < fdCount; j++) {
                            if (j != i) {
                                if (sendall(pfds[j].fd, msg, numBytes) == -1) perror("send");
                            }
                        }
                    }
                }
            }
        }
	}
    // @TODO either actually return other codes above or just make this void?
    return 0;
}

