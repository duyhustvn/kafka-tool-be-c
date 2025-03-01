#include "http_request_header.h"

#include "common.h"
#include "string_util.h"

char* extract_http_request_header(char* s, int n) {
    int http_header_len = 0;
    for (int i = 0; i < n; i++) {
        if (s[i+0] == '\r' && s[i+1] == '\n' &&
            s[i+2] == '\r' && s[i+3] == '\n') {
            http_header_len = i+2;
            break;
        }
    }

    char *http_header_str = malloc(http_header_len+1);
    strncpy(http_header_str, s, http_header_len);
    http_header_str[http_header_len + 1] = '\0';

    return http_header_str;
};


http_request_header *parse_http_request_headers(char *header_str, int header_str_len) {
    if (header_str_len < 3) {
       // at least a:b
       return NULL;
    }

    int cur_number_headers = 0;
    int capacity = 2;


    http_request_header *request_headers = malloc(capacity * sizeof(http_request_header));
    if (request_headers == NULL) {
        return NULL;
    }

    char left = 0;
    char right = 0;
    printf("Headers: \n");
    for (int i = 0; i < header_str_len-1; i++) {
        if (header_str[i] == '\r' && header_str[i+1] == '\n') {
            right = i-1;
            http_request_header *request_header = parse_http_request_header(header_str, left, right);
            if (request_header == NULL) {
                return NULL;
            }
            printf("%s: %s\n", request_header->key, request_header->value);

            left = i+2;
            right = i+3;
        }
    }
    return request_headers;
};


http_request_header *parse_http_request_header(char *header_str, int left_idx, int right_idx) {
    http_request_header *request_header = malloc(sizeof(http_request_header));
    if (request_header == NULL) {
        return NULL;
    }

    for (int i = left_idx; i <= right_idx; i++) {
        if (header_str[i] == ':') {
            // split at first :
            // the line may have more than one :
            int sub_len = i-left_idx;
            char *key = malloc(sizeof(char) * sub_len+1);
            if (key == NULL) {
                return NULL;
            }
            strncpy(key, header_str+left_idx, sub_len);
            key[sub_len] = '\0';
            request_header->key = strdup(key);
            free(key);

            sub_len = right_idx - i;
            char *value = malloc(sizeof(char) * sub_len+1);
            if (value == NULL) {
                return NULL;
            }
            strncpy(value, header_str+i+1, sub_len);
            value[sub_len] = '\0';

            char *trimmed_value = trim_left(value, strlen(value));
            free(value);
            request_header->value = strdup(trimmed_value);
            free(trimmed_value);
        
            return request_header;
        }
    }
    return NULL;
};
