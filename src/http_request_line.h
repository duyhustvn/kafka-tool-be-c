#ifndef HTTP_REQUEST_LINE_H_
#define HTTP_REQUEST_LINE_H_

typedef struct http_request_line_ {
    // sample: GET /api/v1/topics HTTP/1.1
    char *method;
    char *path;
    char *protocol;
} http_request_line;

http_request_line parse_http_request_line(char *request_line, int n);

#endif // HTTP_REQUEST_LINE_H_
