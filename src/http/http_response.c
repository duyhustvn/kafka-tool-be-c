#include "http_response.h"
#include "http_header.h"
#include <sys/socket.h>


void init_http_response(http_response *response) {
    response->status_code = 200; // default set to 200
    strncpy(response->reason_phrase, "OK", sizeof(response->reason_phrase) - 1);
    response->headers = NULL;
    response->header_count = 0;
    response->body = NULL;
    response->body_length = 0;
}


char* construct_http_response(http_response *response, size_t *response_length) {
    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        return NULL;
    }

    // status line
    // status-line = HTTP-version SP status-code SP reason-phrase CRLF
    size_t offset =  snprintf(buffer, buffer_size, "HTTP/1.1 %d %s\r\n", response->status_code, response->reason_phrase);

    // response headers
    for (size_t i = 0; i < response->header_count; i++) {
        // get the length of header
        http_header header = response->headers[i];
        size_t header_length = snprintf(NULL, 0, "%s: %s\r\n", header.key, header.value);
        while (offset + header_length + 1 > buffer_size) {
            buffer_size = 2 * buffer_size;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                return NULL;
            }
        }

        offset += snprintf(buffer+offset, buffer_size-offset, "%s:%s\r\n", header.key, header.value);
    }

    offset += snprintf(buffer+offset, buffer_size-offset, "\r\n");

    if (response->body_length > 0) {
        if (offset + response->body_length + 1 > buffer_size) {
            buffer_size = offset + response->body_length + 1;
            buffer = realloc(buffer, buffer_size);
        }
        offset += snprintf(buffer+offset, buffer_size-offset, "%s", response->body);
    }

    *response_length = offset;
    return buffer;
}


void send_http_response(int client_fd, http_response *response) {
    size_t response_length = 0;
    char *response_data = construct_http_response(response, &response_length);
    if (!response_data) {
        return;
    }

#ifdef DEBUG
    printf("response_data: %s\n", response_data);
#endif

    size_t total_sent = 0;
    while (total_sent < response_length) {
        // https://man7.org/linux/man-pages/man2/send.2.html
        ssize_t bytes_sent = send(client_fd, response_data+total_sent, response_length-total_sent, 0);
        if (bytes_sent <= 0) {
            break;
        }

        total_sent += bytes_sent;
    }

    free(response_data);
}


void free_http_response(http_response *response) {
#ifdef DEBUG
    printf("free http response\n");
#endif

    if (response) {
        free_http_header(response->headers, response->header_count);
        free(response->body);
    }
}
