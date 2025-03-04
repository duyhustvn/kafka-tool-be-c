#include "http_response.h"
#include <string.h>


void init_http_response(http_response *response) {
    response->status_code = 200; // default set to 200
    strncpy(response->reason_phrase, "OK", sizeof(response->reason_phrase) - 1);
    response->headers = NULL;
    response->header_count = 0;
    response->body = NULL;
    response->body_length = 0;
};
