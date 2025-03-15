#include "http_query_string.h"
#include "../common.h"
#include <stdlib.h>
#include <string.h>

int parse_http_query_string(http_query_string **query_strings, size_t *query_string_count, char *query_string, int query_string_len) {
    int key_idx = 0;
    int value_idx = 0;
    char *key = malloc(1);
    char *value = malloc(1);
    for (int i  = 0; i < query_string_len; i++) {
        if (query_string[i] == '=')  {
            int key_len = i - key_idx;
            key = realloc(key, key_len + 1);
            if (key == NULL) {
                return NOT_OK;
            }
            strncpy(key, query_string+key_idx, key_len);
            key[key_len] = '\0';
            value_idx = i+1;
        }

        if (query_string[i] == '&' || i == query_string_len-1) {
            int value_len;
            if (query_string[i] == '&') {
                value_len = i - value_idx;
            } else {
                value_len = i - value_idx + 1;
            }

            value = realloc(value, value_len + 1);
            if (value == NULL) {
                return NOT_OK;
            }
            strncpy(value, query_string+value_idx, value_len);
            value[value_len] = '\0';
            key_idx = i + 1;

            add_http_query_string(query_strings, query_string_count, key, value);
        }
    }

    free(key);
    free(value);

    return OK;
};


int add_http_query_string(http_query_string **query_strings, size_t *query_string_count, char *key, char *value) {
    *query_strings = realloc(*query_strings, (*query_string_count+1) * sizeof(http_query_string));
    if (!query_strings) {
        return NOT_OK;
    }

    (*query_strings)[*query_string_count].key = strdup(key);
    (*query_strings)[*query_string_count].value = strdup(value);

    (*query_string_count)++;
    return OK;
};


void free_http_query_string(http_query_string *query_string) {
    if (query_string) {
        free(query_string->key);
        free(query_string->value);
    }
};


void free_http_query_strings(http_query_string *query_strings, size_t query_string_count) {
    for (int i = 0; i < query_string_count; i++) {
        free_http_query_string(query_strings + i);
    }

    free(query_strings);
};
