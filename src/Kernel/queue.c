#include "queue.h"
#include "stdio.h" // debug purpose

node *init_node(void *payload) {
    node *n = (node *)malloc(sizeof(node));
    n->payload = payload;
    n->next = NULL;

    return n;
}

queue *init_queue() {
    queue *q = (queue *)malloc(sizeof(queue));
    q->head = NULL;
    q->length = 0;

    return q;
}

void add_node(queue *q, node *n) {
    node *prev = q->head;
    // perror("added!\n");
    while (prev && prev->next) {
        prev = prev->next;
    }
    // perror("added!\n");
    if (prev == NULL) {
        // perror("added!\n");
        q->head = n;
    } else {
        prev->next = n;
    }

    q->length++;
}

node *remove_node(queue *q, node *n) {
    node *prev = NULL;
    node *tmp = q->head;

    while (tmp) {
        if (tmp == n) {
            if (prev) {
                prev->next = n->next;
                return n;
            }
            q->head = n->next;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    return n;
}

node *remove_head(queue *q) {
    if (q->head == NULL) {
        perror("ahha\n");
        return q->head;
    }

    node *tmp = q->head;
    q->head = tmp->next;
    q->length--;

    return tmp;
}

void free_queue(queue *q) {
    node *tmp;

    while (q->head) {
        tmp = q->head;
        q->head = tmp->next;
        free(tmp->payload);
        free(tmp);
    }

    free(q);
}