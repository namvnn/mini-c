#ifndef HASH_TABLE_H
#define HASH_TABLE_H

struct ht_entry {
    char *key;
    char *value;
};

struct ht {
    int base_size;
    int size;
    int count;
    struct ht_entry **entries;
};

struct ht *ht_new(void);
char *ht_get(struct ht *ht, const char *key);
void ht_set(struct ht *ht, const char *key, const char *value);
void ht_del(struct ht *ht, const char *key);
void ht_free(struct ht *ht);

#endif /* HASH_TABLE_H */
