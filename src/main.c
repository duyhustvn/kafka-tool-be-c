#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "tcp.h"

int main() {
    tcp_server server = {0};

    server_status_e status = bind_tcp_port(&server, 8080);
    if (status != SERVER_OK) {
        exit(EXIT_FAILURE);
    }

    while (1) {
        int client_fd = accept_client(server.socket_fd);
        if (client_fd == -1) {
            continue;
        }

        printf("Client connected\n");

        read_http_request(client_fd);

        http_response *response = malloc(sizeof(http_response));
        if (response == NULL) {
            continue; 
        }

        init_http_response(response);
        add_http_header(&(response->headers), &(response->header_count), "Content-Type", "application/json");
        add_http_header(&(response->headers), &(response->header_count), "Connection", "close");

        printf("Headers response: \n");
        for (int i = 0; i < response->header_count; i++) {
            printf("%s: %s\n", response->headers[i].key, response->headers[i].value);
        }

        printf("Body of response");
        char response_body[] = "{\"code\": 1}";
        response->body = response_body;
        response->body_length = strlen(response_body);

        send_http_response(client_fd, response);

        free_http_header(response->headers, response->header_count);
        free(response);

        close(client_fd);
    }

    close(server.socket_fd);
    return 0;
}
