#include "unity.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>

// Optional: Code to set up test preconditions.
void setUp(void) {
}

// Optional: Code to clean up after tests.
void tearDown(void) {
    // (Nothing to clean up in this simple example)
}

void test_init_queue(void) {
    queue *q = init_queue();
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_INT(q->size, 0);
    TEST_ASSERT_NULL(q->front);
    TEST_ASSERT_NULL(q->rear);

    free_queue(q);
}

void test_enqueue(void) {
    queue *q = init_queue();
    enqueue(q, "a");
    TEST_ASSERT_EQUAL_INT(q->size, 1);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->value, "a"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->rear->value, "a"), 0);

    enqueue(q, "b");
    TEST_ASSERT_EQUAL_INT(q->size, 2);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->value, "a"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->next->value, "b"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->rear->value, "b"), 0);

    enqueue(q, "c");
    TEST_ASSERT_EQUAL_INT(q->size, 3);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->value, "a"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->next->value, "b"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->next->next->value, "c"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->rear->value, "c"), 0);

    free_queue(q);
}

void test_dequeue_empty_queue(void) {
    queue *q = init_queue();
    node *dequeued_node = dequeue(q);
    TEST_ASSERT_NULL(dequeued_node);

    free_queue(q);
}

void test_dequeue_queue_has_one_element(void) {
    queue *q = init_queue();
    enqueue(q, "a");

    node *dequeued_node = dequeue(q);
    TEST_ASSERT_EQUAL_INT(strcmp(dequeued_node->value, "a"), 0);
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_INT(q->size, 0);
    TEST_ASSERT_NULL(q->front);
    TEST_ASSERT_NULL(q->rear);

    free_queue(q);
}

void test_dequeue_queue_has_two_element(void) {
    queue *q = init_queue();
    enqueue(q, "a");
    enqueue(q, "b");

    node *dequeued_node = dequeue(q);
    TEST_ASSERT_EQUAL_INT(strcmp(dequeued_node->value, "a"), 0);
    TEST_ASSERT_EQUAL_INT(q->size, 1);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->value, "b"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->rear->value, "b"), 0);
    TEST_ASSERT_NULL(q->front->next);
    TEST_ASSERT_NULL(q->rear->next);

    free_queue(q);
}

void test_dequeue_queue_has_multiple_element(void) {
    queue *q = init_queue();
    enqueue(q, "a");
    enqueue(q, "b");
    enqueue(q, "c");
    enqueue(q, "d");

    node *dequeued_node = dequeue(q);
    TEST_ASSERT_EQUAL_INT(strcmp(dequeued_node->value, "a"), 0);
    TEST_ASSERT_EQUAL_INT(q->size, 3);
    TEST_ASSERT_EQUAL_INT(strcmp(q->front->value, "b"), 0);
    TEST_ASSERT_EQUAL_INT(strcmp(q->rear->value, "d"), 0);

    free_queue(q);
}
