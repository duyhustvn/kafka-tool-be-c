#ifndef HTTP_QUERY_STRING_H_
#define HTTP_QUERY_STRING_H_

#include "common.h"

typedef struct http_query_string_ {
    char *key;
    char *value;
} http_query_string;

int parse_http_query_string(http_query_string **query_strings, size_t *query_string_count, char *query_string, int query_string_len);
int add_http_query_string(http_query_string **query_strings, size_t *query_string_count, char *key, char *value);
void free_http_query_string(http_query_string *query_string);
void free_http_query_strings(http_query_string *query_strings, size_t query_string_count);

#endif // HTTP_QUERY_STRING_H_
