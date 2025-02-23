#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

queue* init_queue() {
    queue *q = (queue *)malloc(sizeof(queue));
    if (q == NULL) {
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void enqueue(queue *q, char *v) {
    node new_node = {.value=v, .next=NULL};

    if (q->head == NULL) {
        // queue is empty
        q->head = &new_node;
        q->tail = &new_node;
    } else {
        // queue has item
        new_node.next = q->head;
        q->head = &new_node;
    }

    q->size++;
}

node *dequeue(queue *q) {
    if (q->head == NULL) {
        // queue is empty
        return NULL;
    }

    node *dequeued_node = q->tail;

}
