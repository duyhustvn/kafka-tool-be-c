#include "memcached.h"
#include <stdbool.h>

MemcachedConnection *init_memcached_connection(const char *server_list) {
    MemcachedConnection *conn = (MemcachedConnection*) malloc(sizeof(MemcachedConnection));
    if (!conn) {
        return NULL;
    }

    conn->memc = memcached_create(NULL);
    conn->connected = 0;

    // config servers and options
    memcached_server_st *servers = memcached_servers_parse(server_list);
    memcached_server_push(conn->memc, servers);
    memcached_server_list_free(servers);

    // memcached_behavior_set(conn->memc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
    // memcached_behavior_set(conn->memc, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
    // memcached_behavior_set(conn->memc, MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT, TIMEOUT_MS);
    // memcached_behavior_set(conn->memc, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, TIMEOUT_MS);

    if (memcached_version(conn->memc) == MEMCACHED_SUCCESS) {
        conn->connected = 1;
    }

    if (conn->connected == 0) {
    }

    return conn;
}

char *get_value_from_memcached(MemcachedConnection *conn, const char *key) {
    if (!conn->connected) {
        return NULL;
    }

    size_t value_len;
    uint32_t flags;
    memcached_return_t rc;
    char *value = memcached_get(conn->memc, key, strlen(key), &value_len, &flags, &rc);
    if (rc == MEMCACHED_SUCCESS) {
        return value;
    } else if (rc == MEMCACHED_NOTFOUND) {
        return NOT_FOUND;
    }

    return NULL;
}


bool set_value_into_memcached(MemcachedConnection *conn, const char *key, const char* value, time_t ttl) {
    if (!conn->connected) {
        return false;
    }

    memcached_return_t rc = memcached_set(conn->memc, key, strlen(key), value, strlen(value), ttl, 0);
    if (rc != MEMCACHED_SUCCESS) {
        warnx("SET error: %s", memcached_strerror(conn->memc, rc));
        return false;
    }
    return true;
}
