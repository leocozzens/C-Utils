#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int num;
    struct Node *next;
} Node;

Node *create_node(int val) {
    Node *newNode = malloc(sizeof(Node));
    newNode->num = val;
    newNode->next = NULL;
    return newNode;
}

void insert_first(Node **head, int val) {
    Node *tmp = create_node(val);
    tmp->next = *head;
    *head = tmp;
}

void insert_after(Node *lastNode, Node *newNode) {
    newNode->next = lastNode->next;
    lastNode->next = newNode;
}

Node *find_node(Node **head, int val) {
    Node *tmp = *head;
    while(tmp != NULL) {
        if(tmp->num == val) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

Node *find_prev(Node **head, int val) {
    Node *tmp = *head;
    while(tmp->next != NULL) {
        if(tmp->next->num == val) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void delete_after(Node *prevNode) {
    Node *tmp = prevNode->next;
    prevNode->next = tmp->next;
    free(tmp);
}

void print_list(Node *head) {
    Node *tmp = head;
    while(tmp != NULL) {
        printf("%d - ", tmp->num);
        tmp = tmp->next;
    }
    printf("\n");
}

Node *delete_list(Node **head) {
    Node *tmp = *head;
    if(*head == NULL) return tmp;
    while(tmp->next != NULL) {
        Node *nextNode = tmp->next;
        free(tmp);
        tmp = nextNode;
    }
    *head = NULL;
    return tmp;
}