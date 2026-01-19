#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SHM_NAME "/shm_test"
#define SEM_NAME "/sem_test"
#define DATA_SIZE (100 * 1024 * 1024)

int main() {
    char *buff = malloc(DATA_SIZE);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDONLY, 0666);
    if(shm_fd < 0) {
        perror("Error abriendo la memoria compartida");
        return EXIT_FAILURE;
    }

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if(sem == SEM_FAILED) {
        perror("Error en sem_open");
        return EXIT_FAILURE;
    }    

    void *ptr = mmap(0, DATA_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if(ptr == MAP_FAILED) {
        perror("Error en mmap");
        return EXIT_FAILURE;
    }

    sem_wait(sem);

    memcpy(buff, ptr, DATA_SIZE);

    munmap(ptr, DATA_SIZE);
    sem_close(sem);
    close(shm_fd);
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
    free(buff);

    return EXIT_SUCCESS;
}