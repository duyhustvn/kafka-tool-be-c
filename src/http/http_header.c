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
}

void free_http_header(http_header *headers, size_t header_count) {
    for (size_t i = 0; i < header_count; i++) {
        free(headers[i].key);
        free(headers[i].value);
    }

    free(headers);
}
