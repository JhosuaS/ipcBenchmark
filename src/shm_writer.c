#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>

#define SHM_NAME "/shm_test"
#define SEM_NAME "/sem_test"
#define DATA_SIZE (100 * 1024 * 1024)

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    char *data = malloc(DATA_SIZE);
    for(int i = 0; i < DATA_SIZE; i++) {
        data[i] = '\0';
    }

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if((ftruncate(shm_fd, DATA_SIZE)) < 0) {
        perror("Error en ftruncate");
        return EXIT_FAILURE;
    }

    void *ptr = mmap(0, DATA_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(ptr == MAP_FAILED) {
        perror("Error en mmap");
        return EXIT_FAILURE;
    }

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if(sem == SEM_FAILED) {
        perror("Error en sem_open");
        return EXIT_FAILURE;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    memcpy(ptr, data, DATA_SIZE);
    clock_gettime(CLOCK_MONOTONIC, &end);

    sem_post(sem);

    double time_spent = get_time_diff(start, end);
    FILE *f = fopen("results/metrics.csv", "a");
    if(f != NULL) {
        fprintf(f, "shm,%d,%.6f\n", DATA_SIZE, time_spent);
        fclose(f);
    }

    munmap(ptr, DATA_SIZE);
    sem_close(sem);
    close(shm_fd);
    free(data);

    return EXIT_SUCCESS;
}