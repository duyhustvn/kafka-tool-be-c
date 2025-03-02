#include "unity.h"

#include "common.h"
#include "hashmap.h"

// Optional: Code to set up test preconditions.
void setUp(void) {
}

// Optional: Code to clean up after tests.
void tearDown(void) {
    // (Nothing to clean up in this simple example)
}

void test_hashcode(void) {
    typedef struct Test_ {
        char *input;
        int key_range;
        int expected_output;
    } Test;


    Test tests[] = {
    {.input="a", .key_range=10, .expected_output=9},
    };

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        Test test = tests[i];
        int code = hashcode(test.key_range, test.input);
        TEST_ASSERT_EQUAL_INT(test.expected_output, code);
    }
}
