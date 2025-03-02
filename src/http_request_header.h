#ifndef HTTP_REQUEST_HEADER_H_
#define HTTP_REQUEST_HEADER_H_

typedef struct http_request_header_ {
    char* key;
    char* value;
} http_request_header;

http_request_header *parse_http_request_headers(char *, int /*length of the string*/);
http_request_header *parse_http_request_header(char *, int /*left*/, int /*right*/);

#endif // HTTP_REQUEST_HEADER_H_
