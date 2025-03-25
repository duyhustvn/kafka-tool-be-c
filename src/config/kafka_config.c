#include "kafka_config.h"
#include <stdlib.h>

bool load_kafka_config(KafkaConfig *cfg, const char *errstr) {
    const char* brokers = getenv("KAFKA_BROKERS");
    cfg->brokers = brokers;

    const char* topic = getenv("KAFKA_TOPIC");
    cfg->topic = topic;

    const char* group_id = getenv("KAFKA_GROUP_ID");
    cfg->group_id = group_id;

    const char* poll_timeout_ms_str = getenv("KAFKA_POLL_TIMEOUT_MS");
    int poll_timeout_ms = atoi(poll_timeout_ms_str);
    if (poll_timeout_ms == 0) {
        errstr = "[load_kafka_config] invalid pool_timeout_ms config";
        return false;
    }
    cfg->poll_timeout_ms = poll_timeout_ms;

    return true;
}
