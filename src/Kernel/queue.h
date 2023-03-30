#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include <stdlib.h>

typedef struct ListNode {
    void *payload;
    struct ListNode *next;
} node; 

typedef struct LinkedList {
    node *head;
    int length;
} queue;

node *init_node(void *payload);

queue *init_queue();

void add_node(queue *q, node *n);

node *remove_node(queue *q, node *n);

node *remove_head(queue *q);

void free_queue(queue *q);

#endif