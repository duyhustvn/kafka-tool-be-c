#ifndef HTTP_QUERY_STRING_H_
#define HTTP_QUERY_STRING_H_

#include "common.h"

typedef struct http_query_string_ {
    char *key;
    char *value;
} http_query_string;

int parse_http_query_string(http_query_string **query_strings, char *query_string, int query_string_len);
int add_http_query_string(http_query_string **query_strings, int *num_query_string, char *key, char *value);

#endif // HTTP_QUERY_STRING_H_
