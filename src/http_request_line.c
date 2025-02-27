#include "http_request_line.h"

#include "queue.h"
#include <stdio.h>
#include <unistd.h>

http_request* parse_http_request_line(char *request_line, int len) {
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


    http_request *request = malloc(sizeof(http_request));
    node* method = dequeue(q);
    request->method = strdup(method->value);
    // printf("request method: %s\n", request->method);
    free_node_queue(method);
    node* path = dequeue(q);
    request->path = strdup(path->value);
    // printf("request path: %s\n", request->path);
    free_node_queue(path);
    node* protocol = dequeue(q);
    request->protocol = strdup(protocol->value);
    // printf("request protocol: %s\n", request->protocol);
    free_node_queue(protocol);

    // printf("%s %s %s", request->method, request->path, request->protocol);

    free(q);
    return request;
};

int read_http_request(int socket_fd, http_request *request) {
    char buffer[8192] = {0};

    // https://man7.org/linux/man-pages/man2/read.2.html
    // read() attempts to read up to count bytes from file descriptor fd
    // into the buffer starting at buf
    ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer)-1);
    if (bytes_read <= 0) {
        return -1; // reading failed or connection closed
    }

    buffer[bytes_read] = '\0';
    request = parse_http_request_line(buffer, bytes_read);

    printf("request: %s %s %s", request->method, request->path, request->protocol);

    free(request);

    return 0;
}
