#include "http_request_line.h"


http_request_line parse_http_request_line(char *request_line, int n) {
    int count = 1;
    for (int i = 0; i < n; i++) {
        char c = request_line[i];
        if (c == ' ') {

            count = 1;
        }
        count++;
    }
};
