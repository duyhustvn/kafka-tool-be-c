#include "queue.h"

queue* init_queue() {
    queue *q = (queue *)malloc(sizeof(queue));
    if (q == NULL) {
        return NULL;
    }

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

node *create_node(char *v) {
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->value = strdup(v);
    if (!new_node->value) {
        free(new_node);
        return NULL;
    }
    new_node->next = NULL;
    return new_node;
}

/*
** Add to the rear
*/
void enqueue(queue *q, char *v) {
    node *new_node = create_node(v);

    if (q->front == NULL) {
        // queue is empty
        q->front = new_node;
        q->rear = new_node;
    } else {
        // queue has item
        q->rear->next = new_node;
        q->rear = new_node;
    }

    q->size++;
}

/*
** Pop from the front
*/
node *dequeue(queue *q) {
    if (q->front == NULL) {
        // queue is empty
        return NULL;
    }

    node *dequeued_node = q->front;
    q->front = q->front->next;

    if (q->size == 1) {
        q->front = NULL;
        q->rear = NULL;
    }


    q->size--;

    dequeued_node->next = NULL;
    return dequeued_node;
}

void free_queue(queue *q) {
    if (q) {
        free(q);
    }
}


void free_node_queue(node *n) {
    if (n) {
        free(n->value);
        free(n);
    }
};
