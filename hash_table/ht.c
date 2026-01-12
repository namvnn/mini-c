#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ht.h"
#include "prime.h"

static const int HT__PRIME_1 = 151;
static const int HT__PRIME_2 = 163;
static const int HT__INITIAL_BASE_SIZE = 50;
static struct ht_entry HT__DELETED_ENTRY = { NULL, NULL };
static const int HT__LOAD_RANGE[2] = { 10, 70 };

static struct ht_entry *ht_entry__new(const char *k, const char *v) {
    struct ht_entry *entry = malloc(sizeof(struct ht_entry));
    entry->key = strdup(k);
    entry->value = strdup(v);
    return entry;
}

static void ht__exit_out_of_memory(void) {
    fprintf(stderr, "Out of memory.");
    exit(1);
}

static struct ht *ht__new_with_size(const int base_size) {
    struct ht *ht = malloc(sizeof(struct ht));
    if (ht == NULL) {
        ht__exit_out_of_memory();
    }

    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;

    ht->entries = calloc((size_t)ht->size, sizeof(struct ht_entry *));
    if (ht->entries == NULL) {
        ht__exit_out_of_memory();
    }

    return ht;
}

static void ht__resize(struct ht *ht, const int base_size) {
    if (base_size < HT__INITIAL_BASE_SIZE) {
        return;
    }

    struct ht *new_ht = ht__new_with_size(base_size);
    for (int i = 0; i < ht->size; i++) {
        struct ht_entry *entry = ht->entries[i];
        if (entry != NULL && entry != &HT__DELETED_ENTRY) {
            ht_set(new_ht, entry->key, entry->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;
    ht->size = new_ht->size;

    struct ht_entry **temp_entries = ht->entries;
    ht->entries = new_ht->entries;
    new_ht->entries = temp_entries;

    ht_free(new_ht);
}

static void ht__resize_up(struct ht *ht) {
    const int new_size = ht->base_size * 2;
    ht__resize(ht, new_size);
}

static void ht__resize_down(struct ht *ht) {
    const int new_size = ht->base_size / 2;
    ht__resize(ht, new_size);
}

static void ht_entry__delete(struct ht_entry *entry) {
    free(entry->key);
    free(entry->value);
    free(entry);
}

static int ht__hash(const char *s, const int a, const int m) {
    long h = 0;
    const int len_s = strlen(s);

    for (int i = 0; i < len_s; i++) {
        h += (long)pow(a, len_s - (i + 1)) * s[i];
    }

    h = h % m;
    return (int)h;
}

static int ht__gethash(const char *s, const int num_buckets,
                       const int attempt) {
    const int hash_a = ht__hash(s, HT__PRIME_1, num_buckets);
    if (attempt == 0) {
        return hash_a;
    }
    int hash_b = ht__hash(s, HT__PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

static int ht__calc_load(struct ht *ht) {
    return ht->count * 100 / ht->size;
}

struct ht *ht_new(void) {
    return ht__new_with_size(HT__INITIAL_BASE_SIZE);
}

char *ht_get(struct ht *ht, const char *key) {
    int index = ht__gethash(key, ht->size, 0);
    struct ht_entry *entry = ht->entries[index];

    int i = 1;
    while (entry != NULL && entry != &HT__DELETED_ENTRY) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }

        index = ht__gethash(key, ht->size, i);
        entry = ht->entries[index];
        i++;
    }

    return NULL;
}

void ht_set(struct ht *ht, const char *key, const char *value) {
    if (ht__calc_load(ht) > HT__LOAD_RANGE[1]) {
        ht__resize_up(ht);
    }

    struct ht_entry *entry = ht_entry__new(key, value);

    int index = ht__gethash(entry->key, ht->size, 0);
    struct ht_entry *curr_entry = ht->entries[index];

    int i = 1;
    while (curr_entry != NULL && curr_entry != &HT__DELETED_ENTRY) {
        if (strcmp(curr_entry->key, key) == 0) {
            ht_entry__delete(curr_entry);
            ht->entries[index] = entry;
            return;
        }
        index = ht__gethash(entry->key, ht->size, i);
        curr_entry = ht->entries[index];
        i++;
    }

    ht->entries[index] = entry;
    ht->count++;
}

void ht_del(struct ht *ht, const char *key) {
    if (ht__calc_load(ht) < HT__LOAD_RANGE[0]) {
        ht__resize_down(ht);
    }

    int index = ht__gethash(key, ht->size, 0);
    struct ht_entry *entry = ht->entries[index];

    int i = 1;
    while (entry != NULL && entry != &HT__DELETED_ENTRY) {
        if (strcmp(entry->key, key) == 0) {
            ht_entry__delete(entry);
            ht->entries[index] = &HT__DELETED_ENTRY;
            ht->count--;
            return;
        }

        index = ht__gethash(key, ht->size, i);
        entry = ht->entries[index];
        i++;
    }
}

void ht_free(struct ht *ht) {
    for (int i = 0; i < ht->size; i++) {
        struct ht_entry *entry = ht->entries[i];
        if (entry != NULL && entry != &HT__DELETED_ENTRY) {
            ht_entry__delete(entry);
        }
    }

    free(ht->entries);
    free(ht);
}
