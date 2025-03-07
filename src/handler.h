#ifndef HANDLER_H_
#define HANDLER_H_

#include "common.h"

#include "http_request.h"
#include "http_response.h"

bool handle_request(http_request *request, http_response *response);

void healthcheck_handler(http_request *request, http_response *response);

#endif // HANDLER_H_
