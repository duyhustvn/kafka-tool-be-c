#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct node_ {
    char *value;
    struct node_ *next;
} node;

typedef struct queue_ {
    struct node_ *front;
    struct node_ *rear;
    int size;
} queue;

queue *init_queue();
void enqueue(queue *, char*);
node* dequeue(queue *);
void free_queue(queue *);

#endif // QUEUE_H_
