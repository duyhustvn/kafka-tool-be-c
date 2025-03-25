#include "kafka_consumer.h"

#include <err.h>
#include <librdkafka/rdkafka.h>
#include <stdbool.h>
#include <stdio.h>

static void print_assignment(const rd_kafka_t *rk,
                             rd_kafka_topic_partition_list_t *partitions) {

    printf("%s assigned partitions: ", rd_kafka_name(rk));
    for (int i = 0; i < partitions->cnt; i++) {
        const rd_kafka_topic_partition_t *p = &partitions->elems[i];
        printf("[%d]%s", p->partition, (i != partitions->cnt - 1) ? ", ": "");
    }
    printf("\n");
}

static void rebalance_cb(rd_kafka_t *rk,
                  rd_kafka_resp_err_t err,
                  rd_kafka_topic_partition_list_t *partitions,
                  void *opaque) {
    warnx("Rebalance callback");
    switch (err) {
        case RD_KAFKA_RESP_ERR__ASSIGN_PARTITIONS:
            print_assignment(rk, partitions);
            rd_kafka_assign(rk, partitions);
            break;
        case RD_KAFKA_RESP_ERR__REVOKE_PARTITIONS:
            print_assignment(rk, partitions);
            rd_kafka_assign(rk, NULL);
            break;
        default:
            rd_kafka_assign(rk, NULL);
            warnx("Failed to rebalance partitions: %s", rd_kafka_err2str(err));
    }
}

rd_kafka_conf_t *init_kafka_consumer_config(Config app_conf, char *errstr) {
    rd_kafka_conf_t *conf = rd_kafka_conf_new();

    // set callbacks rebalance
    rd_kafka_conf_set_rebalance_cb(conf, rebalance_cb);

    /*
    ** Set bootstrap broker(s) as a comma-separated list of host or host:port
    ** librdkafka will use the bootstrap brokers to acquire the full set of the brokers
    ** from the cluster
     */
    if (rd_kafka_conf_set(conf, "bootstrap.servers", app_conf.kafka_config->brokers, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        rd_kafka_conf_destroy(conf);
        return NULL;
    }

    /*
    ** Set the consumer group id
    ** All consumers sharing the same group id will join the same group, and the subscribed topic partitions will be assigned
    ** according to the partition.assignment.strategy
    ** (consumer config prorperty) to the consumers in the group
     */
    if (rd_kafka_conf_set(conf, "group.id", app_conf.kafka_config->group_id, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        rd_kafka_conf_destroy(conf);
        return NULL;
    }

    return conf;
}


bool init_kafka_consumer(KafkaConsumer *consumer, Config app_config, char *errstr) {
    rd_kafka_conf_t *cfg = init_kafka_consumer_config(app_config, errstr);
    if (!cfg) {
        return false;
    }
    warnx("Load kafka consumer config successfully");
    consumer->rk = rd_kafka_new(RD_KAFKA_CONSUMER, cfg, errstr, sizeof(errstr));
    if (!consumer->rk) {
        return false;
    }

    rd_kafka_poll_set_consumer(consumer->rk);
    consumer->run = true;
    return true;
}


bool consume_message(KafkaConsumer *consumer, rd_kafka_topic_partition_list_t *topics, int poll_timeout_ms, const char* errstr) {
    warnx("Start consume kafka message with timeout: %d", poll_timeout_ms);

#ifdef DEBUG
    printf("Topics: ");
    for (int i = 0; i < topics->cnt; i++) {
        printf("%s ", topics->elems[i].topic);
    }
    printf("\n");
#endif

    rd_kafka_resp_err_t err = rd_kafka_subscribe(consumer->rk, topics);
    if (err) {
        errstr = rd_kafka_err2str(err);
        return false;
    }

    while (consumer->run) {
        rd_kafka_message_t *msg = rd_kafka_consumer_poll(consumer->rk, poll_timeout_ms);
        if (!msg) {
            continue;
        }

        if (msg->err) {
            switch (msg->err) {
                case RD_KAFKA_RESP_ERR__PARTITION_EOF:
                    errstr = rd_kafka_message_errstr(msg);
                    break;
                default:
                    errstr = rd_kafka_message_errstr(msg);
            }
            warnx("[consume_message] failed %s", errstr);
        } else {
#ifdef DEBUG
        printf("Received message (offset %"PRId64", partition: %d): %.*s\n\n", msg->offset, msg->partition, (int)msg->len, (char*)msg->payload);
#endif
        }

        rd_kafka_message_destroy(msg);
    }
    return true;
}
