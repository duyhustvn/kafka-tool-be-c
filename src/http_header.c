#include "http_header.h"


int add_http_header(http_header **headers, size_t *cur_header_count, const char *key, const char *value) {
    *headers = realloc(*headers, sizeof(http_header) * (*cur_header_count + 1));
    if (headers == NULL) {
        return -1;
    }

    (*headers)[*cur_header_count].key = strdup(key);
    (*headers)[*cur_header_count].value = strdup(value);
    *cur_header_count += 1;
    return 0;
};
