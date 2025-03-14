#ifndef ROUTE_H_
#define ROUTE_H_

#include "./http/http_request.h"
#include "./http/http_response.h"

#define MAX_ROUTES 100

typedef struct route_ {
    char *http_method;
    char *path;
    void (*handler)(http_request*, http_response*);
} route;

int handle_func(char* http_method, char* path, void (*handler)(http_request*, http_response*));

#endif // ROUTE_H_
