#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 6767
#define MAX_CLIENTS 1

typedef struct
{
    int socket;
    char name[32];
    int active;
    pthread_t thread;

} Client;

int find_free_client_spot(Client clients[]){
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active == 0) {
            return i;
        }
    }

    return -1;   
}

void *handle_client(void *arg)
{
    Client *client = (Client *)arg;
    ssize_t valread;
    char buffer[1024] = { 0 };

    while(1){
        valread = recv(client->socket, buffer, sizeof(buffer) - 1, 0);
    
        if (valread == 0) {
            printf("Client %d disconnected\n", client->socket);
            break;
        }
    
        if (valread < 0) {
            perror("recv failed");
            break;
        }

        buffer[valread] = '\0';
        printf("Client %d: %s", client->socket, buffer);
    }

    close(client->socket);
    client->socket = -1;
    client->active = 0;
    client->name[0] = '\0';
    return NULL;
}

int main(void) {
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    Client client_array[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
    client_array[i].socket = -1;
    client_array[i].active = 0;
    client_array[i].name[0] = '\0';
    }

    int client_array_pointer;
    char *refuse_msg = "Server full";
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t send_thread;
    
    while (1) {
        Client new_client;

        if ((new_client.socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        if ((client_array_pointer = find_free_client_spot(client_array)) < 0){
            send(new_client.socket, refuse_msg, strlen(refuse_msg), 0);
            close(new_client.socket);
            continue;
        }

        else {
            new_client.active = 1;
            client_array[client_array_pointer] = new_client;
            printf("Client %d connected\n", client_array[client_array_pointer].socket);
            
            pthread_create(&client_array[client_array_pointer].thread, NULL, handle_client, &client_array[client_array_pointer]);
            pthread_detach(client_array[client_array_pointer].thread);
        }
    }

    close(server_fd);
    
    return 0;
}
