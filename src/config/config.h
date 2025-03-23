#ifndef CONFIG_H_
#define CONFIG_H_

#include "../common.h"
#include "kafka_config.h"

typedef struct Config_ {
    KafkaConfig *kafka_config;
} Config;

bool load_config(Config *config, KafkaConfig *kafka_cfg, const char *errstr);

#endif // CONFIG_H_
