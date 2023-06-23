#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int num;
    struct Node *next;
} Node;

Node *create_node(int val);
void insert_first(Node **head, int val);
void insert_after(Node *lastNode, Node *newNode);
Node *find_node(Node **head, int val);
Node *find_prev(Node **head, int val);
void delete_after(Node *prevNode);
void print_list (Node *head);
Node *delete_list(Node **head);

#endif