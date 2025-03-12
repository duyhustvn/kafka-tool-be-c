#include "http_query_string.h"
#include <stdlib.h>

int parse_http_query_string(http_query_string **query_strings, char *query_string, int query_string_len) {
    int num_query_string = 0;

    int key_idx = 0;
    int value_idx = 0;
    char *key;
    char *value;
    for (int i  = 0; i < query_string_len; i++) {
        if (query_string[i] == '=')  {
            key = malloc(i+1);
            strncpy(key, query_string+key_idx, i);
            key[i] = '\0';
        }

        if (query_string[i] == '&' || i == query_string_len-1) {

            add_http_query_string(query_strings, &num_query_string, key, value);
        }
    }
};
