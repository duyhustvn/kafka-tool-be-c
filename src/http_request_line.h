#ifndef HTTP_REQUEST_LINE_H_
#define HTTP_REQUEST_LINE_H_

#include "common.h"
#include "queue.h"

#define HTTP_METHOD_MAX_LEN 8
#define HTTP_PATH_MAX_LEN 2048
#define HTTP_PROTOCOL_MAX_LEN 16


/*
sample http GET request

GET /api/v1/topics HTTP/1.1\r\n
Content-Type: application/json\r\n
Agent: Firefox/120.1
\r\n
*/
typedef struct http_request_line_ {
    char *method;
    char *path;
    char *protocol;
} http_request_line;

http_request_line* parse_http_request_line(char *request_line, int n);

void free_http_request_line(http_request_line *request);

#endif // HTTP_REQUEST_LINE_H_
