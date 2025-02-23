#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct node_ {
    char *value;
    struct node_ *next;
} node;

typedef struct queue_ {
    struct node_ *head;
    struct node_ *tail;
    int size;
} queue;

queue *init_queue();
void enqueue(queue *, char*);
node* dequeue(queue *);

#endif // QUEUE_H_
