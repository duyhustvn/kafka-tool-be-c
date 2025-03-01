#include "http_request_line.h"

#include "queue.h"
#include "string_util.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

http_request_line* parse_http_request_line(char *request_line, int len) {
    int count = 0;
    int start = 0;

    queue *q = init_queue();

    for (int i = 0; i < len; i++) {
        char c = request_line[i];
        if  (c == ' ') {
            char item[count];
            strncpy(item, request_line + start, count);
            start = i+1;
            item[count] = '\0';
            count = 0;
            enqueue(q, item);
            continue;
        }

        else if (i == len-1) {
            char item[count+1];
            strncpy(item, request_line + start, count+1);
            start = i+1;
            item[count+1] = '\0';
            enqueue(q, item);
            count = 0;
            continue;
        }
        count++;
    }

#ifdef DEBUG
    printf("queue size: %d\n", q->size);
#endif

    if (q->size < 3) {
#ifdef DEBUG
        printf("len: %d\n", len);
        printf("request_line: %s\n", request_line);
        int qs = q->size;
        for (int i = 0; i < qs; i++) {
            node* nd = dequeue(q);
            printf("node value: %s\n", nd->value);
            free_node_queue(nd);
        }
#endif
        free(q);
        return NULL;
    }


    http_request_line *request = malloc(sizeof(http_request_line));
    node* method = dequeue(q);
    request->method = strdup(method->value);
    free_node_queue(method);

    node* path = dequeue(q);
    request->path = strdup(path->value);
    free_node_queue(path);

    node* protocol = dequeue(q);
    request->protocol = strdup(protocol->value);
    free_node_queue(protocol);

    free_queue(q);
    return request;
};


http_request_line* read_http_request(int socket_fd) {
    char buffer[8192] = {0};

    // https://man7.org/linux/man-pages/man2/read.2.html
    // read() attempts to read up to count bytes from file descriptor fd
    // into the buffer starting at buf
    ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer)-1);
    if (bytes_read <= 0) {
        return NULL; // reading failed or connection closed
    }

    buffer[bytes_read] = '\0';

    size_t buffer_len = strlen(buffer);

    int http_request_line_len = 0;
    for (int i = 0; i < buffer_len; i++) {
        if (buffer[i] == '\r' && buffer[i+1] == '\n') {
            http_request_line_len = i;
            break;
        }
    }
#ifdef DEBUG
    printf("buffer: %s\n", buffer);
#endif

    char http_request_line_str[http_request_line_len+1];
    strncpy(http_request_line_str, buffer, http_request_line_len);
    http_request_line_str[http_request_line_len+1] = '\0';

    cut_off_first_n_character(buffer, http_request_line_len+2);

#ifdef DEBUG
    printf("http_request_line_str: %s len %lu\n", http_request_line_str, strlen(http_request_line_str));
    printf("buffer: %s\n", buffer);
#endif

    http_request_line *request = parse_http_request_line(http_request_line_str, bytes_read);

    // free_http_request(request);

    return request;
}


void free_http_request(http_request_line *request) {
    if (request) {
#ifdef DEBUG
        printf("free_http_request\n");
#endif
        free(request->method);
        free(request->path);
        free(request->protocol);
        free(request);
    }
};
