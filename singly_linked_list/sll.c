#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "sll.h"

static void sll__exit_out_of_memory(void) {
    fprintf(stderr, "Out of memory.");
    exit(1);
}

struct node *node_new(int data) {
    struct node *node = malloc(sizeof(struct node));

    if (node == NULL) {
        sll__exit_out_of_memory();
    }

    node->data = data;
    node->next = NULL;

    return node;
}

struct sll *sll_new(void) {
    struct sll *list = malloc(sizeof(struct sll));

    if (list == NULL) {
        sll__exit_out_of_memory();
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

struct node *sll_get_head(struct sll *list) {
    return list->head;
}

struct node *sll_get_tail(struct sll *list) {
    return list->tail;
}

void sll_insert_head(struct sll *list, struct node *node) {
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head = node;
    }
}

void sll_insert_tail(struct sll *list, struct node *node) {
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

void sll_remove_head(struct sll *list) {
    if (list->head == NULL) {
        return;
    }

    struct node *head = list->head;
    list->head = head->next;
    head->next = NULL;
    free(head);
}

void sll_remove_tail(struct sll *list) {
    if (list->head == NULL) {
        return;
    }

    struct node *penultimate = NULL;

    for (struct node *current = list->head; current->next != NULL;
         current = current->next) {
        penultimate = current;
    }

    struct node *tail = list->tail;
    penultimate->next = NULL;
    list->tail = penultimate;
    free(tail);
}

void sll_traverse(struct sll *list) {
    for (struct node *current = list->head; current; current = current->next) {
        if (current->next != NULL) {
            printf("%d -> ", current->data);
        } else {
            printf("%d", current->data);
        }
    }
}

void sll_foreach(struct sll *list, void (*func)(struct node *)) {
    for (struct node *current = list->head; current; current = current->next) {
        func(current);
    }
}

bool sll_contain(struct sll *list, struct node *node) {
    for (struct node *current = list->head; current; current = current->next) {
        if (current->data == node->data) {
            return true;
        }
    }

    return false;
}

void sll_clear(struct sll *list) {
    if (list->head == NULL) {
        return;
    }

    struct node *current = list->head;
    do {
        struct node *cleared_node = current;
        current = current->next;
        free(cleared_node);
    } while (current != NULL);

    list->head = NULL;
    list->tail = NULL;
}

void sll_delete(struct sll *list) {
    sll_clear(list);
    free(list);
}
