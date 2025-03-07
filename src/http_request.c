#include "http_request.h"
#include "common.h"
#include "hashmap.h"
#include "http_request_header.h"
#include "string_util.h"
#include "queue.h"

int read_http_request(int socket_fd, http_request *request) {
    char buffer[8192] = {0};

    // https://man7.org/linux/man-pages/man2/read.2.html
    // read() attempts to read up to count bytes from file descriptor fd
    // into the buffer starting at buf
    ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer)-1);
    if (bytes_read <= 0) {
        return NOT_OK; // reading failed or connection closed
    }

    buffer[bytes_read] = '\0';

    size_t buffer_len = strlen(buffer);

    http_request_component component = parse_http_request_component(buffer, buffer_len);

    int res = extract_http_request_line(request, component.request_line_start, component.request_line_length);
    if (res == NOT_OK) {
        printf("extract_http_request_line failed");
        return NOT_OK;
    }
    printf("Request line: \n%s %s %s\n", request->method, request->path, request->protocol);

    hashmap *headers = parse_http_request_headers(component.request_headers_start, component.request_headers_length);
    if (headers == NULL) {
        return NOT_OK;
    }
    request->headers = headers;

#ifdef DEBUG
    printf("Headers: \n");
    char *hostname = get(headers, "Host");
    printf("Hostname: %s\n", hostname);
    char *user_agent = get(headers, "User-Agent");
    printf("User-Agent: %s\n", user_agent);
    char *accept = get(headers, "Accept");
    printf("Accept: %s\n", accept);
#endif 

    return OK;
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

int extract_http_request_line(http_request *request, char *request_line, int len) {
    int count = 0;
    int start = 0;

    queue *q = init_queue();
    if (!q) {
        return NOT_OK;
    }

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
        return NOT_OK;
    }


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
    return OK;
};

int free_http_request(http_request *request) {
    if (request) {
#ifdef DEBUG
        printf("free http request line\n");
#endif
        free(request->method);
        free(request->path);
        free(request->protocol);
        free_hashmap(request->headers);
        // free(request);
    }

    return OK;
};
