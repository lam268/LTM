#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define MAXCHAR 1000
#define backlog 3

void *client_handler(void *arg)
{
    char server_message[100], client_message[100];
    int client_sock;
    client_sock = (int)arg;
    pthread_detach(pthread_self());
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    // Check Account:
    do
    {
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
        {
            printf("Couldn't receive\n");
            return EXIT_FAILURE;
        }
        printf("Client message: %s\n", client_message);
        if (client_message[0] == '\0' || strcmp(client_message, "bye") == 0)
        {
            printf("Couldn't receive\n");
            return EXIT_FAILURE;
        }
        strcpy(server_message, "next");
        send(client_sock, server_message, strlen(server_message), 0);
    } while (true);

    // Closing the socket:
    close(client_sock);
}

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    pthread_t tid;
    char data[MAXCHAR];
    FILE *fPtr;
    int client = 0;
    char clientstr[20];
    if (argc == 1)
    {
        printf("Please input port number!!!\n");
        return 0;
    }

    int port = atoi(argv[1]);

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    // Listen for clients:
    if (listen(socket_desc, backlog) < 0)
    {
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");

    while (1)
    {
        // Accept an incoming connection:
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);
        if (client_sock < 0)
        {
            printf("Cant accept\n");
            return -1;
        }
        printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        pthread_create(&tid, NULL, &client_handler, (void *)client_sock);
        client++;
        sprintf(clientstr, "%d",client);
        fPtr = fopen(strcat(clientstr,"client.txt"), "w");

        /* fopen() return NULL if last operation was unsuccessful */
        if (fPtr == NULL)
        {
            /* File not created hence exit */
            printf("Unable to create file.\n");
            exit(EXIT_FAILURE);
        }
        fputs(data, fPtr);

        /* Close file to save file data */
        fclose(fPtr);
    }
    close(socket_desc);
    client = 0;
    return 0;
}