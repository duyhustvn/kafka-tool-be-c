#ifndef KAFKA_CONSUMER_H_
#define KAFKA_CONSUMER_H_

#include "../common.h"
#include <librdkafka/rdkafka.h>
#include "../config/config.h"

typedef struct KafkaConsumer_ {
    rd_kafka_t *rk;
} KafkaConsumer;

rd_kafka_conf_t *init_kafka_consumer_config(Config app_conf, char *errstr);
bool init_kafka_consumer(KafkaConsumer *consumer, Config app_config, char *errstr);


#endif // KAFKA_CONSUMER_H_
