#include <queue.h>

void make_queue(Queue *init) {
    init->head = NULL;
    init->tail = NULL;
}

void enqueue(Queue *queue, int val) {
    Node *currentNode = malloc(sizeof(Node));
    if(currentNode == NULL) { // Optionally you could return a boolean and loop in the parent process but I will keep things simple here
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(1);
    }
    currentNode->val = val;
    currentNode->nextNode = NULL;
    if(queue->tail != NULL) queue->tail->nextNode = currentNode;
    else queue->head = currentNode;
    queue->tail = currentNode;
}

_Bool dequeue(Queue *queue, int *retVal) {
    if (queue->head == NULL) return 0;
    Node *tmp = queue->head;
    queue->head = tmp->nextNode;
    if(queue->head == NULL) queue->tail = NULL;
    *retVal = tmp->val;

    free(tmp);
    return 1;
}