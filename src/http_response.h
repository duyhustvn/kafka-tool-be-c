#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include "common.h"
#include "http_header.h"

/*
The first line of a response message is the status-line, consisting
of the protocol version, a space (SP), the status code, another
space, a possibly empty textual phrase describing the status code,
and ending with CRLF.

    status-line = HTTP-version SP status-code SP reason-phrase CRLF

* */
typedef struct http_response_ {
    int status_code; // http status code 200, 400, 500 ...
    char reason_phrase[64]; // Text explaination of the status ("OK", "Not Found" ...)
    http_header *headers; // Array of HTTP headers
    size_t header_count; // Number of headers
    char *body; // Body content
    size_t body_length; // Length of the response body
} http_response;

void init_http_response(http_response *response);

char* construct_http_response(http_response *response, size_t *response_length);
void send_http_response(int client_fd, http_response *response);

#endif // HTTP_RESPONSE_H_
