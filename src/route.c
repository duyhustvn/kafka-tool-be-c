#include "route.h"
#include <string.h>

route routes[MAX_ROUTES];
int route_count = 0;

int handle_func(char* http_method, char* path, void (*handler)(http_request*, http_response*)) {
    for (int i = 0; i < MAX_ROUTES; i++) {
        routes[route_count].http_method = http_method;
        routes[route_count].path = path;
        routes[route_count].handler = handler;
        route_count++;
    }

    return route_count;
};
