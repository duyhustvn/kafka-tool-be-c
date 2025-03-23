#include "kafka_config.h"
#include <stdlib.h>

bool load_kafka_config(KafkaConfig *cfg, const char *errstr) {
    const char* brokers = getenv("KAFKA_BROKERS");
    cfg->brokers = brokers;

    const char* topic = getenv("KAFKA_TOPIC");
    cfg->topic = topic;

    const char* group_id = getenv("KAFKA_GROUP_ID");
    cfg->group_id = group_id;

    const char* pool_timeout_ms_str = getenv("KAFKA_POOL_TIMEOUT_MS");
    int pool_timeout_ms = atoi(pool_timeout_ms_str);
    if (pool_timeout_ms == 0) {
        errstr = "[load_kafka_config] invalid pool_timeout_ms config";
        return false;
    }
    cfg->pool_timeout_ms = pool_timeout_ms;

    return true;
};
