#include "tcp.h"

#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


/*
** create socket, bind it to  a port
** then start listening for connection
*/
server_status_e bind_tcp_port(tcp_server *server, int port) {
    memset(server, 0, sizeof(*server));

    // https://man7.org/linux/man-pages/man2/socket.2.html
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd == -1) {
        printf("Failed to create the socket");
        return SERVER_SOCKET_ERROR;
    }

    int opt = 1;
    // SO_REUSEADDR allow the port to be used immediately after the server stop
    if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        printf("Failed to set socket options");
        return SERVER_SOCKET_ERROR;
    }

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);

    // https://man7.org/linux/man-pages/man2/bind.2.html
    int bind_result = bind(server->socket_fd, (struct sockaddr*) &server->address, sizeof(server->address));
    if (bind_result < 0) {
        printf("Failed to bind the socket");
        close(server->socket_fd);
        return SERVER_BIND_ERROR;
    }

    // https://man7.org/linux/man-pages/man2/listen.2.html
    int listen_result = listen(server->socket_fd, LISTEN_BACKLOG);
    if (listen_result < 0) {
        printf("Failed to listen the socket");
        close(server->socket_fd);
        return SERVER_LISTEN_ERROR;
    }

    printf("Server bound and listening on port %d\n", port);
    return SERVER_OK;
}


/*
** Accept incoming connection
*/
int accept_client(int server_fd) {
    struct sockaddr_in client_address = {0};
    socklen_t client_len = sizeof(client_address);

    int client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_len);
    if (client_fd < 0) {
        printf("Failed to accept");
        return -1;
    }

    return client_fd;
}
