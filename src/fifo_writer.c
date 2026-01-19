#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h> 

#define DATA_SIZE (100 * 1024 * 1024)
#define FIFO_NAME "fifo_test"

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    char *data = malloc(DATA_SIZE);
    for(int i = 0; i < DATA_SIZE; i++) {
        data[i] = '\0';
    }

    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Error creando FIFO");
            return EXIT_FAILURE;
        }
    }

    printf("Esperando lectores...\n");
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Error abriendo FIFO");
        return EXIT_FAILURE;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    write(fd, data, DATA_SIZE);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent = get_time_diff(start, end);
    FILE *f = fopen("results/metrics.csv", "a");
    if(f != NULL) {
        fprintf(f, "named_pipe,%d,%.6f\n", DATA_SIZE, time_spent);
        fclose(f);
    }

    unlink(FIFO_NAME); 

    return EXIT_SUCCESS;
}