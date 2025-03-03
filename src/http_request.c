#include "http_request.h"
#include "hashmap.h"
#include "http_request_header.h"
#include "string_util.h"
#include "http_request_line.h"
#include <string.h>

int read_http_request(int socket_fd) {
    char buffer[8192] = {0};

    // https://man7.org/linux/man-pages/man2/read.2.html
    // read() attempts to read up to count bytes from file descriptor fd
    // into the buffer starting at buf
    ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer)-1);
    if (bytes_read <= 0) {
        return -1; // reading failed or connection closed
    }

    buffer[bytes_read] = '\0';

    size_t buffer_len = strlen(buffer);

    http_request_component component = parse_http_request_component(buffer, buffer_len);

    http_request_line *request_line = parse_http_request_line(component.request_line_start, component.request_line_length);
    printf("Request line: \n%s %s %s\n", request_line->method, request_line->path, request_line->protocol);

    hashmap *headers = parse_http_request_headers(component.request_headers_start, component.request_headers_length);
    if (headers == NULL) {
        return -1;
    }

#ifdef DEBUG
    printf("Headers: \n");
    char *hostname = get(headers, "Host");
    printf("Hostname: %s\n", hostname);
    char *user_agent = get(headers, "User-Agent");
    printf("User-Agent: %s\n", user_agent);
    char *accept = get(headers, "Accept");
    printf("Accept: %s\n", accept);
#endif 


    free_http_request_line(request_line);
    free_hashmap(headers);


    return -1;
}


http_request_component parse_http_request_component(char* request, int request_length) {
    http_request_component component = {0};

    component.request_line_start = request;
    char *request_line = request;
    while (!(*request_line == '\r' && *(request_line + 1) == '\n')) {
        request_line++;
    }
    component.request_line_length = request_line - request;


    component.request_headers_start = request_line + 2; 

    char *request_headers = request_line + 2;
    while (!(*request_headers == '\r' &&  *(request_headers+1) == '\n' &&
        *(request_headers+2) == '\r' && *(request_headers+3) == '\n')
    ) {
        request_headers++;
    }
    component.request_headers_length = request_headers - component.request_headers_start + 2; // include "\r\n" at the end of headers

    return component;
};
