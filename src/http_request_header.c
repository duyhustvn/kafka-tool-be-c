#include "http_request_header.h"

#include "string_util.h"

hashmap *parse_http_request_headers(char *header_str, int header_str_len) {
    if (header_str_len < 3) {
       // at least a:b
       return NULL;
    }

    hashmap *headers = init_hashmap();
    if (headers == NULL) {
        return NULL;
    }

    char left = 0;
    char right = 0;

    for (int i = 0; i < header_str_len-1; i++) {
        if (header_str[i] == '\r' && header_str[i+1] == '\n') {
            right = i-1;
            int result = parse_http_request_header(header_str, left, right, headers);
            if (result == -1) {
                return NULL;
            }

            left = i+2;
            right = i+3;
        }
    }
    return headers;
};


int parse_http_request_header(char *header_str, int left_idx, int right_idx, hashmap *headers) {
    for (int i = left_idx; i <= right_idx; i++) {
        if (*(header_str+i) == ':') {
            // split at first :
            // the line may have more than one :
            int sub_len = i-left_idx;

            char key[sub_len+1];
            strncpy(key, header_str+left_idx, sub_len);
            key[sub_len] = '\0';

            sub_len = right_idx - i;
            char value[sub_len+1];
            strncpy(value, header_str+i+1, sub_len);
            value[sub_len] = '\0';
            char *trimmed_value = trim_left(value, strlen(value));

            int resutl = insert(headers, key, trimmed_value);
            if (resutl == -1) {
                printf("failed to insert to hashmap\n");
            }

            free(trimmed_value);
            break;
        }
    }
    return 1;
};
