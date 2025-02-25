#include "unity.h"
#include "string_util.h"
#include <string.h>
#include <stdlib.h>

typedef struct Test_ {
        char *input;
        char *expected_output;
    } Test;

// Optional: Code to set up test preconditions.
void setUp(void) {

}

// Optional: Code to clean up after tests.
void tearDown(void) {
    // (Nothing to clean up in this simple example)
}

void test_trim_left(void) {
    Test tests[] = {
    {.input="abc",.expected_output="abc"},
    {.input=" abc",.expected_output="abc"},
    {.input="  abc",.expected_output="abc"},
    {.input="   abc",.expected_output="abc"},
    {.input="   abc ",.expected_output="abc "},
    {.input="   abc  ",.expected_output="abc  "},
    };

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        Test test = tests[i];
        char *s = trim_left(test.input, strlen(test.input));
        TEST_ASSERT_EQUAL_STRING(test.expected_output, s);
        free(s);
    }
}

void test_trim_right(void) {
    Test tests[] = {
    {.input="abc",.expected_output="abc"},
    {.input="abc ",.expected_output="abc"},
    {.input="abc  ",.expected_output="abc"},
    {.input="abc   ",.expected_output="abc"},
    {.input=" abc   ",.expected_output=" abc"},
    };

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        Test test = tests[i];
        char *s = trim_right(test.input, strlen(test.input));
        TEST_ASSERT_EQUAL_STRING(test.expected_output, s);
        free(s);
    }
}
