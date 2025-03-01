#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http_request_line.h"
#include "queue.h"
#include "tcp.h"

int main() {
    tcp_server server = {0};

    server_status_e status = bind_tcp_port(&server, 8080);
    if (status != SERVER_OK) {
        exit(EXIT_FAILURE);
    }

    int client_fd = accept_client(server.socket_fd);
    if (client_fd == -1) {
        close(server.socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    http_request_line *request = read_http_request(client_fd);

    printf("request: %s %s %s\n", request->method, request->path, request->protocol);
    free_http_request(request);

    close(server.socket_fd);
    close(client_fd);
    return 0;
}

// int main() {
//     char * str = "GET /index.html HTTP/1.1";
//     http_request_line *res = parse_http_request_line(str, strlen(str));
//     printf("method: %s| ", res->method);
//     printf("path: %s| ", res->path);
//     printf("protocol: %s| ", res->protocol);
//     printf("\n");
//
//     str = "POST /api/v1/list HTTP/1.1";
//     res = parse_http_request_line(str, strlen(str));
//     printf("method: %s| ", res->method);
//     printf("path: %s| ", res->path);
//     printf("protocol: %s| ", res->protocol);
//     printf("\n");
// }
