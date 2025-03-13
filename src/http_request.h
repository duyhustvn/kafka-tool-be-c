#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "common.h"

#include "hashmap.h"
#include "http_header.h"
#include "http_query_string.h"


typedef struct http_request_ {
    char *method;
    char *path;
    char *protocol;
    http_query_string *query_strings;
    size_t query_string_count;
    hashmap *headers;
    char *body;
} http_request;

typedef struct http_request_component_ {
    char* request_line_start;
    size_t request_line_length;

    char* request_headers_start;
    size_t request_headers_length;

    char* request_body_start;
    size_t request_body_length;
} http_request_component;

int read_http_request(int socket_fd, http_request *request);
http_request_component parse_http_request_component(char* request, int request_length);

int extract_http_request_line(http_request *request, char *request_line, int len);

int free_http_request(http_request *request);


#endif // HTTP_REQUEST_H_
