#include <stdio.h>

#include "sll.h"

int main(void) {
    struct sll *list = sll_new();

    printf("insert head");
    for (int i = 5; i >= 1; i--) {
        printf(" %d", i);
        sll_insert_head(list, node_new(i));
    }
    printf("\n");
    sll_traverse(list);
    printf("\n\n");

    printf("insert tail");
    for (int i = 6; i <= 10; i++) {
        printf(" %d", i);
        sll_insert_tail(list, node_new(i));
    }
    printf("\n");
    sll_traverse(list);
    printf("\n\n");

    printf("remove head\n");
    sll_remove_head(list);
    sll_traverse(list);
    printf("\n\n");

    printf("remove tail\n");
    sll_remove_tail(list);
    sll_traverse(list);
    printf("\n\n");
}
