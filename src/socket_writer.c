#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#define SOCKET_NAME "socket_test.sock"
#define DATA_SIZE (100 * 1024 * 1024)

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    char *data = malloc(DATA_SIZE);
    for(int i = 0; i < DATA_SIZE; i++) {
        data[i] = '\0';
    }

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error en socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX; 
    strcpy(addr.sun_path, SOCKET_NAME);

    unlink(SOCKET_NAME);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Error en bind");
        return EXIT_FAILURE;
    }

    if((listen(server_fd, 1)) < 0) {
        perror("Error en listen");
        return EXIT_FAILURE;
    }
    printf("Esperando conexiones en el socket...\n");

    int client_fd = accept(server_fd, NULL, NULL);
    if(client_fd < 0) {
        perror("Error en accept");
        return EXIT_FAILURE;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    if((send(client_fd, data, DATA_SIZE, 0)) < 0) {
        perror("Error enviando datos");
        return EXIT_FAILURE;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent = get_time_diff(start, end);
    FILE *f = fopen("results/metrics.csv", "a");
    if(f != NULL) {
        fprintf(f, "socket,%d,%.6f\n", DATA_SIZE, time_spent);
        fclose(f);
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_NAME);
    free(data);
    
    return EXIT_SUCCESS;
}