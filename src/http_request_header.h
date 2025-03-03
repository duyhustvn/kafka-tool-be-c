#ifndef HTTP_REQUEST_HEADER_H_
#define HTTP_REQUEST_HEADER_H_

#include "common.h"
#include "hashmap.h"

typedef struct http_request_header_ {
    char* key;
    char* value;
} http_request_header;

hashmap *parse_http_request_headers(char *, int /*length of the string*/);
int parse_http_request_header(char *, int /*left*/, int /*right*/, hashmap *);

#endif // HTTP_REQUEST_HEADER_H_
