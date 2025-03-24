#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./http/http_method.h"
#include "./http/http_request.h"
#include "./http/http_response.h"
#include "config/config.h"
#include "config/kafka_config.h"
#include "handler.h"
#include "kafka/kafka_consumer.h"
#include "route.h"
#include "tcp.h"

int main() {
    tcp_server server = {0};

    Config config = {0};
    KafkaConfig kafka_config = {0};
    char errstr[512];

    if (!load_config(&config, &kafka_config, errstr)) {
        warnx("%s", errstr);
        exit(EXIT_FAILURE);
    };
    warnx("Load app config successfully");

    KafkaConsumer kafka_consumer = {0};
    if (!init_kafka_consumer(&kafka_consumer, config, errstr)) {
        warnx("%s", errstr);
        exit(EXIT_FAILURE);
    }
    warnx("Init kafka consumer successfully");

    int topic_cnt = 1;
    const char *topic = kafka_config.topic;
    const char **topics = &topic;
    /* Convert the list of topics to a format suitable for librdkafka */
    rd_kafka_topic_partition_list_t *topic_partitions = rd_kafka_topic_partition_list_new(topic_cnt);
    for (int i = 0; i < topic_cnt; i++) {
        rd_kafka_topic_partition_list_add(topic_partitions, topics[i], /*The parition is ignored by subscribe()*/ RD_KAFKA_PARTITION_UA);
    }
    bool consume_res = consume_message(&kafka_consumer, topic_partitions, config.kafka_config->pool_timeout_ms, errstr);
    if (!consume_res) {
        errx(EXIT_FAILURE, "Consume kafka message failed: %s", errstr);
    }

    server_status_e status = bind_tcp_port(&server, 8080);
    if (status != SERVER_OK) {
        exit(EXIT_FAILURE);
    }

    handle_func(HTTP_METHOD_GET, "/api/v1/healthz", healthcheck_handler);

    while (1) {
        int client_fd = accept_client(server.socket_fd);
        if (client_fd == -1) {
            continue;
        }

        printf("Client connected\n");

        http_request request = {0};
        read_http_request(client_fd, &request);

        http_response response = {0};
        init_http_response(&response);

        if (!handle_request(&request, &response)) {
            response.status_code = 404;
            strncpy(response.reason_phrase, "NOT FOUND", strlen("NOT FOUND"));
            response.reason_phrase[strlen("NOT FOUND")] = '\0';
        };

        send_http_response(client_fd, &response);


        close(client_fd);

        // free request
        free_http_request(&request);

        // free response
        free_http_response(&response);
    }

    close(server.socket_fd);
    return 0;
}
