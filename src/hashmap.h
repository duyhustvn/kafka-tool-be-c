#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "common.h"

typedef struct item_ {
    char *key;
    char *value;

    struct item_ *next;
} item;

typedef struct hashmap_ {
    int key_range;
    item *buckets;
} hashmap;

hashmap *init_hashmap();
int insert(hashmap *h, char *key, char *value);
int hashcode(int key_range, char *key);


#endif // HASHMAP_H_
