#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define DATA_SIZE (100 * 1024 * 1024) 

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    int fd[2]; 
    pid_t pid;
    char *data = malloc(DATA_SIZE);
    for(int i = 0; i < DATA_SIZE; i++) {
        data[i] = '\0';
    }

    if(pipe(fd) < 0) {
        perror("Error al crear el pipe");
        return EXIT_FAILURE;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork falló");
        return EXIT_FAILURE;
    }

    if (pid > 0) {
        close(fd[0]);

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        write(fd[1], data, DATA_SIZE);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_spent = get_time_diff(start, end);

        FILE *f = fopen("results/metrics.csv", "a");
        if(f != NULL) {
            fprintf(f, "unnamed_pipe,%d,%.6f\n", DATA_SIZE, time_spent);
            fclose(f);
        }

        close(fd[1]);
        wait(NULL);
    } else {
        close(fd[1]);

        int readed_bytes = 0;
        while(readed_bytes != DATA_SIZE) {
            int bytes_read = read(fd[0], data + readed_bytes, DATA_SIZE - readed_bytes);
            if(bytes_read < 0) {
                perror("Error al leer del pipe");
                break;
            }else if (bytes_read == 0)
            {
                perror("Pipe cerrado inesperadamente");
                break;
            }
            
            readed_bytes += bytes_read;
        }
        close(fd[0]);
        free(data);
    }

return EXIT_SUCCESS;
}