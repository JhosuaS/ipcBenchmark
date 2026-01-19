#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <mqueue.h>

#define MQ_NAME "/mq_test"
#define DATA_SIZE (100 * 1024 * 1024)

int main() {
    char *buff = malloc(DATA_SIZE);
    struct mq_attr attr;

    mqd_t q = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0666, NULL);
    if(q == (mqd_t) - 1) {
        perror("Error creando la cola de mensajes");
        return EXIT_FAILURE;
    }

    if((mq_getattr(q, &attr)) < 0) {
        perror("Error obteniendo atributos de la cola");
        mq_close(q);
        return EXIT_FAILURE;
    }

    size_t total_bytes = 0;
    while(1) {
        ssize_t bytes_received = mq_receive(q, buff + total_bytes, attr.mq_msgsize, NULL);
        if(bytes_received < 0) {
            perror("Error recibiendo mensaje");
            mq_close(q);
            free(buff);
            return EXIT_FAILURE;
        }

        total_bytes += bytes_received;
        if(total_bytes >= DATA_SIZE) {
            break;
        }
    }

    mq_close(q);
    mq_unlink(MQ_NAME);
    free(buff);

    return EXIT_SUCCESS;
}