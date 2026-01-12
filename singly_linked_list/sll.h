#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <stdbool.h>

struct node {
    int data;
    struct node *next;
};

struct sll {
    struct node *head;
    struct node *tail;
};

struct node *node_new(int val);

struct sll *sll_new(void);
struct node *sll_get_head(struct sll *list);
struct node *sll_get_tail(struct sll *list);
void sll_insert_head(struct sll *list, struct node *node);
void sll_insert_tail(struct sll *list, struct node *node);
void sll_remove_head(struct sll *list);
void sll_remove_tail(struct sll *list);
void sll_traverse(struct sll *list);
void sll_foreach(struct sll *list, void (*func)(struct node *));
bool sll_contain(struct sll *list, struct node *node);
void sll_clear(struct sll *list);
void sll_delete(struct sll *list);

#endif /* SINGLY_LINKED_LIST_H */
