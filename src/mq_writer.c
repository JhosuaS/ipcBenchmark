#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <mqueue.h>

#define MQ_NAME "/mq_test"
#define DATA_SIZE (100 * 1024 * 1024)

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    struct mq_attr attr;
    char *data = malloc(DATA_SIZE);
    for(int i = 0; i < DATA_SIZE; i++) {
        data[i] = '\0';
    }
    
    mqd_t q = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0666, NULL);
    if(q == (mqd_t) - 1) {
        perror("Error creando la cola de mensajes");
        return EXIT_FAILURE;
    }

    if((mq_getattr(q, &attr)) < 0) {
        perror("Error obteniendo atributos de la cola");
        mq_close(q);
        return EXIT_FAILURE;
    }

    int bytes_sent = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    while(bytes_sent < DATA_SIZE) {
        size_t to_send = (DATA_SIZE - bytes_sent) < attr.mq_msgsize ? (DATA_SIZE - bytes_sent) : attr.mq_msgsize;
        if((mq_send(q, data + bytes_sent, to_send, 0)) < 0) {
            perror("Error enviando mensaje");
            mq_close(q);
            return EXIT_FAILURE;
        }
        bytes_sent += to_send;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent = get_time_diff(start, end);
    FILE *f = fopen("results/metrics.csv", "a");
    if(f != NULL) {
        fprintf(f, "mq,%d,%.6f\n", DATA_SIZE, time_spent);
        fclose(f);
    }

    mq_close(q);
    free(data);

    return EXIT_SUCCESS;
}
