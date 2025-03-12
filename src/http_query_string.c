#include "http_query_string.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

int parse_http_query_string(http_query_string **query_strings, char *query_string, int query_string_len) {
    int num_query_string = 0;

    int key_idx = 0;
    int value_idx = 0;
    char *key;
    char *value;
    for (int i  = 0; i < query_string_len; i++) {
        if (query_string[i] == '=')  {
            int key_len = i - key_idx +1;
            key = realloc(key, key_len + 1);
            if (key == NULL) {
                return NOT_OK;
            }
            strncpy(key, query_string+key_idx, key_len);
            key[key_len] = '\0';
            value_idx = i+1;
        }

        if (query_string[i] == '&' || i == query_string_len-1) {
            int value_len = i - value_idx;
            value = realloc(value, value_len + 1);
            if (value == NULL) {
                return NOT_OK;
            }
            strncpy(value, query_string+value_idx, value_len);
            value[value_len] = '\0';
            key_idx = i + 1;

            add_http_query_string(query_strings, &num_query_string, key, value);
        }
    }

    return OK;
};


int add_http_query_string(http_query_string **query_strings, int *num_query_string, char *key, char *value) {

    return OK;
};
