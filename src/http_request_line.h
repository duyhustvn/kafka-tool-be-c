#ifndef HTTP_REQUEST_LINE_H_
#define HTTP_REQUEST_LINE_H_

#include "common.h"
#include "queue.h"

#define HTTP_METHOD_MAX_LEN 8
#define HTTP_PATH_MAX_LEN 2048
#define HTTP_PROTOCOL_MAX_LEN 16

typedef struct http_request_ {
    // sample: GET /api/v1/topics HTTP/1.1
    char *method;
    char *path;
    char *protocol;
} http_request;

http_request* parse_http_request_line(char *request_line, int n);

int read_http_request(int socket_fd, http_request *request);

#endif // HTTP_REQUEST_LINE_H_
