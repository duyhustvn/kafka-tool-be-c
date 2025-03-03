#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "common.h"

typedef struct item_ {
    char *key;
    char *value;

    struct item_ *next;
    struct item_ *previous;
} item;

typedef struct linked_list_ {
    item *head;
    item *tail;
    int size;
} linked_list;

typedef struct hashmap_ {
    int key_range;
    linked_list **buckets;
} hashmap;

item *create_item(char *key, char* value);
linked_list *init_linked_list();
hashmap *init_hashmap();
int insert(hashmap *h, char *key, char *value);
int hashcode(int key_range, char *key);


#endif // HASHMAP_H_
