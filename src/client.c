#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 6767
volatile int running = 1;

void *send_thread(void *arg){
    int client_fd = *(int *)arg;
    char msg[1024];

    while(running) {
        if (fgets(msg, sizeof(msg), stdin) == NULL){
            printf("Input closed\n");
            break;
        }

        if (strcmp(msg, "/quit\n") == 0){
            printf("exiting chat...\n");
            running = 0;
            shutdown(client_fd, SHUT_RDWR);
            break;
        }

        if (send(client_fd, msg, strlen(msg), 0) < 0){
            printf("send failed\n");
            break;
        }
    }

    return NULL;
}

void *recv_thread(void *arg){
    int client_fd = *(int *)arg;
    char buffer[1024] = { 0 };
    ssize_t valread;

    while (running){
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

    running = 0;
    return NULL;

}

int main(void) {
    int client_fd;
    struct sockaddr_in server_address;
    
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_address.sin_port = htons(SERVER_PORT);

    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    pthread_t thread_send;
    pthread_t thread_recv;

    pthread_create(&thread_send, NULL, send_thread, &client_fd);
    pthread_create(&thread_recv, NULL, recv_thread, &client_fd);

    pthread_join(thread_send, NULL);
    pthread_join(thread_recv, NULL);

    close(client_fd);

    return 0; 
}