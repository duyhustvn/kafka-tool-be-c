#ifndef KAFKA_CONSUMER_H_
#define KAFKA_CONSUMER_H_

#include "../common.h"

#include <librdkafka/rdkafka.h>
#include "../config/config.h"

typedef struct KafkaConsumer_ {
    rd_kafka_t *rk;
    volatile sig_atomic_t run;
} KafkaConsumer;

static void print_partition_list(FILE *fp,
                                 const rd_kafka_topic_partition_list_t *partitions);
static void rebalance_cb(rd_kafka_t *rk,
                         rd_kafka_resp_err_t err,
                         rd_kafka_topic_partition_list_t *partitions,
                         void *opaque);
rd_kafka_conf_t *init_kafka_consumer_config(Config app_conf, char *errstr);
bool init_kafka_consumer(KafkaConsumer *consumer, Config app_config, char *errstr);
bool consume_message(KafkaConsumer *consumer, rd_kafka_topic_partition_list_t *topics, int poll_timeout_ms, const char* errstr);


#endif // KAFKA_CONSUMER_H_
