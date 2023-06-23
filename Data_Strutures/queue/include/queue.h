#ifndef QUEUE_H
#define QUEUE_H

// C standard libraries
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *nextNode;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} Queue;

void make_queue(Queue *init);
void enqueue(Queue *queue, int val);
_Bool dequeue(Queue *queue, int *retVal);

#endif