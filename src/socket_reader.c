#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_NAME "socket_test.sock"
#define DATA_SIZE (100 * 1024 * 1024)

int main() {
    char *buff = malloc(DATA_SIZE);

    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        perror("Error creando socket");
        free(buff);
        return EXIT_FAILURE;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX; 
    strcpy(addr.sun_path, SOCKET_NAME);

    if((connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
        perror("Error en connect");
        close(sock_fd);
        free(buff);
        return EXIT_FAILURE;
    }

    int readed_bytes = 0;
    while(readed_bytes != DATA_SIZE) {
        int bytes_read = recv(sock_fd, buff + readed_bytes, DATA_SIZE - readed_bytes, 0);
        if(bytes_read < 0) {
            perror("Error al leer del socket");
            break;
        }else if (bytes_read == 0)
        {
            perror("Socket cerrado inesperadamente");
            break;
        }
            readed_bytes += bytes_read;
    }

    close(sock_fd);
    free(buff);
    return EXIT_SUCCESS;
    
}