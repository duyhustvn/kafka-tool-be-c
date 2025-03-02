#include "hashmap.h"
#include <stdlib.h>

hashmap *init_hashmap() {
    hashmap *h = malloc(sizeof(hashmap));
    if (h == NULL) {
        return NULL;
    }

    h->key_range = 100;
    h->buckets = malloc(h->key_range * sizeof(item));
    if (h->buckets == NULL) {
        return NULL;
    }
    return h;
};

int hashcode(int key_range, char *key) {
    int code = 0;
    while (*key != '\0') {
        code += *key;
    }

    return code % key_range;
}

int insert(hashmap *h, char *key, char *value){
    int bucket_id = hashcode(h->key_range, key);
    return 1;
};
