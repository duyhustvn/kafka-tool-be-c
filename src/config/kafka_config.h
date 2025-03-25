#ifndef KAFKA_CONFIG_H_
#define KAFKA_CONFIG_H_

#include "../common.h"

typedef struct KafkaConfig_ {
    const char* brokers;
    const char* topic;
    const char* group_id;
    int poll_timeout_ms;
} KafkaConfig;

bool load_kafka_config(KafkaConfig *cfg, const char *errstr);

#endif // KAFKA_CONFIG_H_
