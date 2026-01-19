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

int main() {
    char *buff = malloc(DATA_SIZE);

    int fd = open(FIFO_NAME, O_RDONLY);
    if(fd < 0) {
        perror("Error abriendo FIFO");
        free(buff);
        return EXIT_FAILURE;
    }

    int readed_bytes = 0;
    while(readed_bytes != DATA_SIZE) {
        int bytes_read = read(fd, buff + readed_bytes, DATA_SIZE - readed_bytes);
        if(bytes_read < 0) {
            perror("Error al leer del FIFO");
            break;
        }else if (bytes_read == 0)
        {
            perror("FIFO cerrado inesperadamente");
            break;
        }
            readed_bytes += bytes_read;
    }

    free(buff);
    close(fd);
    return EXIT_SUCCESS;
}