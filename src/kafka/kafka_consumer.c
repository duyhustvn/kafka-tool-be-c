#include "kafka_consumer.h"

#include <err.h>
#include <librdkafka/rdkafka.h>
#include <stdbool.h>
#include <stdio.h>

static void print_partition_list(FILE *fp,
                                 const rd_kafka_topic_partition_list_t *partitions) {
        int i;
        for (i = 0; i < partitions->cnt; i++) {
                warnx("%s [%" PRId32 "] offset %" PRId64,
                      partitions->elems[i].topic,
                      partitions->elems[i].partition,
                      partitions->elems[i].offset);
        }
}

static void rebalance_cb(rd_kafka_t *rk,
                         rd_kafka_resp_err_t err,
                         rd_kafka_topic_partition_list_t *partitions,
                         void *opaque) {
        rd_kafka_error_t *error     = NULL;
        rd_kafka_resp_err_t ret_err = RD_KAFKA_RESP_ERR_NO_ERROR;

        const char *topic = NULL;
        rd_kafka_topic_t *rkt = NULL;

        warnx("Consumer group rebalanced");

        switch (err) {
        case RD_KAFKA_RESP_ERR__ASSIGN_PARTITIONS:
                warnx("assigned (%s):\n", rd_kafka_rebalance_protocol(rk));
                print_partition_list(stderr, partitions);

                if (!strcmp(rd_kafka_rebalance_protocol(rk), "COOPERATIVE")) {
                    error = rd_kafka_incremental_assign(rk, partitions);
                } else {
                    ret_err = rd_kafka_assign(rk, partitions);
                }

                // Seek to end of all assigned partitions
                if (partitions->cnt > 0) {

                    topic = partitions->elems[0].topic;
                    rkt = rd_kafka_topic_new(rk, topic, NULL);

                    if (!rkt) {
                        warnx("Failed to create topic handle for %s", topic);
                        break;
                    }

                    // Seek to the end of all partition to skip existing messages
                    for (int i = 0; i < partitions->cnt; i++) {
                        const rd_kafka_topic_partition_t *p = &partitions->elems[i];
                        rd_kafka_resp_err_t seek_err;
                        // seek to the end with 1s timeout
                        seek_err = rd_kafka_seek(rkt, p->partition, RD_KAFKA_OFFSET_END, 1000);

                        if (seek_err) {
                            warnx("Seek failed for %s [%d]: %s", p->topic, p->partition, rd_kafka_err2str(seek_err));
                        } else {
                            warnx("Seek %s [%d] to the END (offset %d)", p->topic, p->partition, RD_KAFKA_OFFSET_END);
                        }
                    }

                    rd_kafka_topic_destroy(rkt);
                }
                break;

        case RD_KAFKA_RESP_ERR__REVOKE_PARTITIONS:
                warnx("revoked (%s):\n", rd_kafka_rebalance_protocol(rk));
                print_partition_list(stderr, partitions);

                if (!strcmp(rd_kafka_rebalance_protocol(rk), "COOPERATIVE")) {
                    error = rd_kafka_incremental_unassign(rk, partitions);
                } else {
                    ret_err  = rd_kafka_assign(rk, NULL);
                }
                break;
        default:
                warnx("failed: %s\n", rd_kafka_err2str(err));
                rd_kafka_assign(rk, NULL);
                break;
        }

        if (error) {
                warnx("incremental assign failure: %s", rd_kafka_error_string(error));
                rd_kafka_error_destroy(error);
        } else if (ret_err) {
                warnx("assign failure: %s", rd_kafka_err2str(ret_err));
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
            if (msg->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
                warnx("Consumer reached end of topic %s [%" PRId32"] message queue at offset %" PRId64 "\n", rd_kafka_topic_name(msg->rkt), msg->partition, msg->offset);
            }

            if (msg->rkt) {
                warnx("Consume error for topic %s [%" PRId32"] offset %" PRId64 ": %s\n", rd_kafka_topic_name(msg->rkt), msg->partition, msg->offset, rd_kafka_message_errstr(msg));
            } else {
                warnx("Consume error for topic %s: %s\n", rd_kafka_err2str(msg->err), rd_kafka_message_errstr(msg));
            }

            if (msg->err == RD_KAFKA_RESP_ERR__UNKNOWN_PARTITION || msg->err == RD_KAFKA_RESP_ERR__UNKNOWN_TOPIC) {
                warnx("Consume error topic or partition doesn't exists");
            }
        } else {
#ifdef DEBUG
        //printf("Received message (offset %"PRId64", partition: %d): %.*s\n\n", msg->offset, msg->partition, (int)msg->len, (char*)msg->payload);
#endif
        }

        rd_kafka_message_destroy(msg);
    }
    return true;
}
