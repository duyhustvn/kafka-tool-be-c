#include "http_request_line.h"

#include "queue.h"

http_request_line* parse_http_request_line(char *request_line, int n) {
    int count = 0;
    int start = 0;

   queue *q = init_queue();

    for (int i = 0; i < n; i++) {
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

        else if (i == n-1) {
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

    if (q->size != 3) {
        return NULL;
    }

    http_request_line *request = malloc(sizeof(http_request_line));
    node* method = dequeue(q);
    request->method = method->value;
    node* path = dequeue(q);
    request->path = path->value;
    node* protocol = dequeue(q);
    request->protocol = protocol->value;

    return request;
};
