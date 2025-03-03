#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

linked_list *init_linked_list() {
    linked_list *ll = malloc(sizeof(linked_list));
    if (ll == NULL) {
        return NULL;
    }

    ll->head = ll->tail = NULL;

    ll->size = 0;
    return ll;
}

hashmap *init_hashmap() {
    hashmap *h = malloc(sizeof(hashmap));
    if (h == NULL) {
        return NULL;
    }

    h->key_range = 100;
    h->buckets = malloc(h->key_range * sizeof(linked_list));
    if (h->buckets == NULL) {
        return NULL;
    }

    for (int i = 0; i < h->key_range; i++) {
        h->buckets[i] = init_linked_list();
    }
    return h;
};

int hashcode(int key_range, char *key, int key_length) {
    int code = 0;

    for (int i = 0; i < key_length; i++) {
        code += key[i];
    }

    return code % key_range;
}

int insert(hashmap *h, char *key, char *value){
    int bucket_id = hashcode(h->key_range, key, strlen(key));

#ifdef DEBUG
    printf("insert key: %s value: %s into bucket: %d\n", key, value, bucket_id);
#endif

    linked_list *bucket = h->buckets[bucket_id];
    item *cur = bucket->head;
    while (cur != NULL) {
        if (strncmp(key, cur->key, strlen(key)) == 0) {
            // Found the key
            // update it
            cur->value = value;
            return 1;
        }
        cur = cur->next;
    }

    // Not found the key
    // insert it
    item *new_item = create_item(key, value);
    if (new_item == NULL) {
        return -1;
    }
    if (bucket->head == NULL) {
        bucket->head = bucket->tail = new_item;
    } else {
       bucket->tail->next = new_item;
       new_item->previous = bucket->tail;
       bucket->tail = new_item;
    }

    bucket->size++;

    return 1;
};

item *create_item(char *key, char *value) {
    item *new_item = malloc(sizeof(item));
    if (new_item == NULL) {
        return NULL;
    }

    new_item->key = strdup(key);
    new_item->value = strdup(value);
    new_item->next = NULL;
    new_item->previous = NULL;
    return new_item;
}


char *get(hashmap *h, char *key) {
    int bucket_id = hashcode(h->key_range, key, strlen(key));

    linked_list *bucket = h->buckets[bucket_id];
    item *cur = bucket->head;

    while (cur != NULL) {
        if (strncmp(key, cur->key, strlen(key)) == 0) {
            // Found the key
            return cur->value;
        }
        cur = cur->next;
    }
    return NULL;
};

void free_linked_list(linked_list *ll) {
    if (ll) {
        item *tmp = ll->head;
        while (ll->size != 0) {
            tmp = tmp->next;
            free(ll->head->key);
            free(ll->head->value);
            free(ll->head);
            ll->head = tmp;
            ll->size--;
        }

        free(ll);
    }
}

void free_hashmap(hashmap *h) {
    if (h) {
        for (int i = 0; i < h->key_range; i++) {
            linked_list *bucket = h->buckets[i];
            if (bucket) {
                free_linked_list(bucket);
            }
        }

        free(h->buckets);
        free(h);
    }
};
