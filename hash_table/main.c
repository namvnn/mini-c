#include <stdio.h>

#include "ht.h"

int main(void) {
    struct ht *ht = ht_new();

    printf("hash table size: %d\n", ht->size);

    printf("before:\n");

    ht_set(ht, "name", "John");
    ht_set(ht, "age", "18");
    ht_set(ht, "phone", "0123456789");
    printf("name: %s\n", ht_get(ht, "name"));
    printf("age: %s\n", ht_get(ht, "age"));
    printf("phone: %s\n", ht_get(ht, "phone"));
    printf("hash table size: %d\n", ht->size);

    printf("after:\n");

    ht_set(ht, "name", "Leo");
    ht_set(ht, "age", "19");
    ht_del(ht, "phone");
    printf("name: %s\n", ht_get(ht, "name"));
    printf("age: %s\n", ht_get(ht, "age"));
    printf("phone: %s\n", ht_get(ht, "phone"));
    printf("hash table size: %d\n", ht->size);

    ht_free(ht);
}
