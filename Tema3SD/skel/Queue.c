#include <stdlib.h>

#include "Queue.h"

void init_q(struct Queue *q) {
    q->list = malloc(sizeof(struct LinkedList));
    if (q->list == NULL) {
        return;
    }

    init_list(q->list);
}

int get_size_q(struct Queue *q) {
    if (q->list == NULL) {
        return -1;
    }
    return q->list->size;
}

int is_empty_q(struct Queue *q) {
    if (q->list->size == 0) {
        return 1;
    }
    return 0;
}

void* front(struct Queue *q) {
    if (q->list == NULL || q->list->size == 0) {
        return NULL;
    }
    return q->list->head->data;
}

void dequeue(struct Queue *q) {
    struct Node *curr;
    if (q->list == NULL || q->list->size == 0) {
        return;
    }
    curr = remove_nth_node(q->list, 0);
    free(curr);
}

void enqueue(struct Queue *q, void *new_data) {
    if (q->list == NULL) {
        return;
    }
    add_nth_node(q->list, q->list->size, new_data);
}

void clear_q(struct Queue *q) {
    while (get_size_q(q) > 0) {
        dequeue(q);
    }
}

void purge_q(struct Queue *q) {
    if (q->list == NULL) {
        return;
    }
    clear_q(q);
    free_list(&q->list);
}
