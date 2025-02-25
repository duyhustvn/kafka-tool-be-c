#include "unity.h"

#include "http_request_line.h"


typedef struct Test_ {
    char* input;
    http_request_line expected;
} Test;

// Optional: Code to set up test preconditions.
void setUp(void) {
}

// Optional: Code to clean up after tests.
void tearDown(void) {
    // (Nothing to clean up in this simple example)
}

void test_parse_http_request_line() {
    Test tests[] = {
    {
    .input="GET /test1 HTTP/1.1",
    .expected={.method="GET",.path="/test1",.protocol="HTTP/1.1"},
    },
    };

    int n = sizeof(tests)/sizeof(tests[0]);
    for (int i = 0; i < n; i++) {
        Test test = tests[i];

        http_request_line *request_line = parse_http_request_line(test.input, sizeof(test.input));
        TEST_ASSERT_EQUAL_STRING(test.expected.method, request_line->method);
        free(request_line);
    }
}
