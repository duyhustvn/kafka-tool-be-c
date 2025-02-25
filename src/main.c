#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http_request_line.h"
#include "tcp.h"

// int main() {
//     tcp_server server = {0};
//
//     server_status_e status = bind_tcp_port(&server, 8081);
//     if (status != SERVER_OK) {
//         exit(EXIT_FAILURE);
//     }
//
//     int client_fd = accept_client(server.socket_fd);
//     if (client_fd == -1) {
//         close(server.socket_fd);
//         exit(EXIT_FAILURE);
//     }
//
//     printf("Client connected");
//
//     close(server.socket_fd);
//     close(client_fd);
//     return 0;
// }

int main() {
    char * str = "GET /index.html HTTP/1.1";
    http_request_line *res = parse_http_request_line(str, strlen(str));
    printf("method: %s| ", res->method);
    printf("path: %s| ", res->path);
    printf("protocol: %s| ", res->protocol);
    printf("\n");

    str = "POST /api/v1/list HTTP/1.1";
    res = parse_http_request_line(str, strlen(str));
    printf("method: %s| ", res->method);
    printf("path: %s| ", res->path);
    printf("protocol: %s| ", res->protocol);
    printf("\n");
}
