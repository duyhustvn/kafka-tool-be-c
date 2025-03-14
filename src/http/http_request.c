#include "http_request.h"
#include "common.h"
#include "hashmap.h"
#include "http_query_string.h"
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

    hashmap *headers = parse_http_request_headers(component.request_headers_start, component.request_headers_length);
    if (headers == NULL) {
        return NOT_OK;
    }
    request->headers = headers;

#ifdef DEBUG
    printf("Request line: \n%s %s %s\n", request->method, request->path, request->protocol);

    printf("Query string\n");
    for (int i = 0; i < request->query_string_count; i++) {
        printf("%s: %s\n", request->query_strings[i].key, request->query_strings[i].value);
    }

    printf("Headers: \n");
    char *hostname = get(headers, "Host");
    printf("Hostname: %s\n", hostname);
    char *user_agent = get(headers, "User-Agent");
    printf("User-Agent: %s\n", user_agent);
    char *accept = get(headers, "Accept");
    printf("Accept: %s\n", accept);

    printf("Body: \n");
    for (int i = 0; i < component.request_body_length; i++)  {
        printf("%c", *(component.request_body_start+i));
    }
    printf("\n");
#endif 

    return OK;
}


http_request_component parse_http_request_component(char* request, int request_length) {
    int offset = 0;

    http_request_component component = {0};

    // http request line
    component.request_line_start = request;
    char *request_line = request;
    while (!(*request_line == '\r' && *(request_line + 1) == '\n')) {
        request_line++;
    }
    component.request_line_length = request_line - request;
    offset += component.request_line_length + 2;

    // http headers
    component.request_headers_start = request_line + 2;
    char *request_headers = request_line + 2;
    while (!(*request_headers == '\r' &&  *(request_headers+1) == '\n' &&
        *(request_headers+2) == '\r' && *(request_headers+3) == '\n')
    ) {
        request_headers++;
    }
    component.request_headers_length = request_headers - component.request_headers_start + 2; // include "\r\n" before the new line "\r\n"
    offset += component.request_headers_length + 2;

    // http body
    if (offset >= request_length) {
        // has no body
        return component;
    }

    component.request_body_start = request_headers + 4;
    component.request_body_length = request_length - offset;
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
    char *path_str = path->value;
    bool has_query_string = false;
    char *query_string_start;
    int query_string_len = 0;
    for (int i  = 0; i < strlen(path_str); i++) {
        if (path_str[i] == '?') {
            has_query_string = true;
            request->path = malloc(i+1);
            strncpy(request->path, path_str, i);
            request->path[i] = '\0';

            query_string_start = path_str + i  + 1;
            query_string_len = strlen(path_str) - i - 1;
            break;
        }
    }
    if (!has_query_string) {
        request->path = strdup(path_str);
    } else {
        parse_http_query_string(&request->query_strings, &request->query_string_count, query_string_start, query_string_len);
    }

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
        printf("free http request\n");
#endif
        free(request->method);
        free(request->path);
        free(request->protocol);
        free_hashmap(request->headers);
        free_http_query_strings(request->query_strings, request->query_string_count);
        // free(request);
    }

    return OK;
};
