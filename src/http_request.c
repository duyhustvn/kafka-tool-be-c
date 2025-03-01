#include "http_request.h"
#include "http_request_header.h"
#include "string_util.h"
#include "http_request_line.h"

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


#ifdef DEBUG
    printf("original buffer: %s\n", buffer);
#endif

    int http_request_line_len = 0;
    for (int i = 0; i < buffer_len-1; i++) {
        if (buffer[i] == '\r' && buffer[i+1] == '\n') {
            http_request_line_len = i;
            break;
        }
    }

    char http_request_line_str[http_request_line_len+1];
    strncpy(http_request_line_str, buffer, http_request_line_len);
    http_request_line_str[http_request_line_len+1] = '\0';
#ifdef DEBUG
    printf("http_request_line_str: %s len %lu\n", http_request_line_str, strlen(http_request_line_str));
#endif

    cut_off_first_n_character(buffer, http_request_line_len+2);

#ifdef DEBUG
    printf("buffer remain length: %lu\n%s\n", strlen(buffer), buffer);
#endif

    // Extract the header
    buffer_len = strlen(buffer);
    char *http_header_str = extract_http_request(buffer, buffer_len);

#ifdef DEBUG
    printf("http header: \n%s\n\n", http_header_str);
#endif

    http_request_line *request = parse_http_request_line(http_request_line_str, bytes_read);

    printf("http request line: %s %s %s\n", request->method, request->path, request->protocol);
    free_http_request_line(request);

    free(http_header_str);

    return -1;
}
