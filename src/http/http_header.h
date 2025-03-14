#ifndef HTTP_HEADER_H_
#define HTTP_HEADER_H_

#include "../common.h"

typedef struct http_header_ {
    char *key;
    char *value;
} http_header;

int add_http_header(http_header **headers, size_t *cur_header_count, const char *key, const char *value);
void free_http_header(http_header *headers, size_t header_count);

#endif // HTTP_HEADER_H_
