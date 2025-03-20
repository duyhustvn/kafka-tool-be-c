#ifndef MEMCACHED_H_
#define MEMCACHED_H_

#include "../common.h"

#include <libmemcached/memcached.h>

typedef struct MemcachedConnection_ {
    memcached_st *memc;
    unsigned connected;
} MemcachedConnection;

MemcachedConnection *init_memcached_connection(const char *servers);
char *get_value_from_memcached(MemcachedConnection *conn, const char *key);
bool set_value_into_memcached(MemcachedConnection *conn, const char *key, const char* value, time_t ttl);

#endif // MEMCACHED_H_
