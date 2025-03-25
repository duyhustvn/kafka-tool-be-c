#include "config.h"
#include "kafka_config.h"

bool load_config(Config *config, KafkaConfig *kafka_cfg, const char *errstr) {
    if (!load_kafka_config(kafka_cfg, errstr)) {
        return false;
    }

    config->kafka_config = kafka_cfg;
    return true;
}
