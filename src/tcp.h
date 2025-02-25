#ifndef TCP_H_
#define TCP_H_

#include <arpa/inet.h>

#include "common.h"

#define LISTEN_BACKLOG 50

typedef enum {
SERVER_OK,
SERVER_SOCKET_ERROR,
SERVER_BIND_ERROR,
SERVER_LISTEN_ERROR,
SERVER_ACCEPT_ERROR,
} server_status_e;

typedef struct tcp_server_ {
    int socket_fd; // file descriptor
    struct sockaddr_in address;
} tcp_server;

server_status_e bind_tcp_port(tcp_server *server, int port);
int accept_client(int server_fd);

#endif // TCP_H_
