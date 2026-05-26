#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 6767

int main(void) {
    int client_fd;
    ssize_t valread;
    struct sockaddr_in server_address;
    char buffer[1024] = { 0 };

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_address.sin_port = htons(SERVER_PORT);

    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    char* msg = "Test 123";
    send(client_fd, msg, sizeof(msg), 0);

    while(1) {
        valread = recv(client_fd, buffer, sizeof(buffer) -1, 0);

        if (valread == 0) {
            printf("server closed\n");
            break;
        }

        if (valread < 0) {
            printf("recv failed");
            break;
        }

        buffer[valread] = '\0';
        printf("Server: %s", buffer);
    }

    return 0; 
};