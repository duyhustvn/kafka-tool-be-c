#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tcp.h"

int main() {
    tcp_server server = {0};

    server_status_e status = bind_tcp_port(&server, 8081);
    if (status != SERVER_OK) {
        exit(EXIT_FAILURE);
    }

    int client_fd = accept_client(server.socket_fd);
    if (client_fd == -1) {
        close(server.socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected");

    close(server.socket_fd);
    close(client_fd);
    return 0;
}
