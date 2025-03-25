#include "handler.h"
#include "route.h"

extern route routes[];
extern int route_count;

bool handle_request(http_request *request, http_response *response) {
    for (int i = 0; i < route_count; i++) {
        if (strcmp(routes[i].path, request->path) == 0 && strcmp(routes[i].http_method, request->method) == 0) {
            routes[i].handler(request, response);
            return true;
        }
    }

    return false;
}


void healthcheck_handler(http_request *request, http_response *response) {
    add_http_header(&(response->headers), &(response->header_count), "Content-Type", "application/json");
    add_http_header(&(response->headers), &(response->header_count), "Connection", "close");

    printf("Headers response: \n");
    for (size_t i = 0; i < response->header_count; i++) {
        printf("%s: %s\n", response->headers[i].key, response->headers[i].value);
    }

    printf("Body of response\n");
    char response_body[] = "{\"status\": \"ok\"}";
    response->body = strdup(response_body);
    response->body_length = strlen(response_body);
}
