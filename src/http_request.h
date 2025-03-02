#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "common.h"
#include <stddef.h>

typedef struct http_request_component_ {
    char* request_line_start;
    size_t request_line_length;

    char* request_headers_start;
    size_t request_headers_length;

    char* request_body_start;
    size_t request_body_length;
} http_request_component;

int read_http_request(int socket_fd);
http_request_component parse_http_request_component(char* request, int request_length);

#endif // HTTP_REQUEST_H_
