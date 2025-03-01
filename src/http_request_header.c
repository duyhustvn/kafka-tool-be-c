#include "http_request_header.h"

#include "common.h"

char* extract_http_request(char* s, int n) {
    int http_header_len = 0;
    for (int i = 0; i < n; i++) {
        if (s[i+0] == '\r' && s[i+1] == '\n' &&
            s[i+2] == '\r' && s[i+3] == '\n') {
            http_header_len = i;
            break;
        }
    }

    char *http_header_str = malloc(http_header_len+1);
    strncpy(http_header_str, s, http_header_len);
    http_header_str[http_header_len + 1] = '\0';

    return http_header_str;
};
