#include "unity.h"

#include "common.h"
#include "http_request_header.h"

typedef struct Test_ {
    char* input;
    char* expected;
} Test;

// Optional: Code to set up test preconditions.
void setUp(void) {
}

// Optional: Code to clean up after tests.
void tearDown(void) {
    // (Nothing to clean up in this simple example)
}

void test_extract_http_request() {
 Test tests[] = {
    {
    .input="Host: localhost:8080\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n",
    .expected="Host: localhost:8080\r\nUser-Agent: curl/7.81.0\r\nAccept: */*",
    },
    };

    int n = sizeof(tests)/sizeof(tests[0]);
    for (int i = 0; i < n; i++) {
        Test test = tests[i];

        char *request_header_str = extract_http_request(test.input, strlen(test.input));
        TEST_ASSERT_NOT_NULL(request_header_str);
        // TEST_MESSAGE(request_header_str);
        // TEST_ASSERT_EQUAL_STRING(test.expected, request_header);
        TEST_ASSERT_EQUAL_INT(strlen(test.expected), strlen(request_header_str));
        free(request_header_str);
    }
}
