#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Please input IP address and port number\n");
        return 0;
    }
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000];
    char client_message[2000];
    char null[20] = "\0";
    int port = atoi(argv[2]);

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Unable to create socket\n");
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Unable to connect\n");
        return EXIT_FAILURE;
    }
    printf("Connected with server successfully\n");

    // CheckAccount
    do
    {
        memset(client_message, '\0', sizeof(client_message));
        // Get input from the user:
        printf("Enter message: ");
        gets(client_message);

        if (client_message[0] == '\0' || strcmp(client_message, "bye") == 0)
        {
            return EXIT_FAILURE;
        }

        // Send the message to server:
        send(socket_desc, client_message, strlen(client_message), 0);

        // Receive the server's response:
        recv(socket_desc, server_message, sizeof(server_message), 0);
    } while (true);
    // Close the socket:
    close(socket_desc);

    return 0;
}