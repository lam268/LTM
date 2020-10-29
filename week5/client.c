#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000];
    char password[60], username[20];
    char null[20] = "\0";
    char new_password[20];
    int port = atoi(argv[2]);

    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));
    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    memset(new_password, '\0', sizeof(new_password));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    
    printf("Socket created successfully\n");
    
    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    
    // Send connection request to server:
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");
    
    // CheckAccount
    do {
        memset(username, '\0', sizeof(username));
        // Get input from the user:
        printf("Enter username: ");
        gets(username);

        if (username[0] == '\0' || strcmp(username, "bye") == 0){
            return -1;
        }

        // Send the message to server:
        send(socket_desc, username, strlen(username),0);
    
        // Receive the server's response:
        recv(socket_desc, server_message, sizeof(server_message),0);
    } while (strcmp(server_message,"Insert password\n") != 0);

    // CheckPassword
    do {
        memset(password, '\0', sizeof(password));
        printf("Insert password: ");
        gets(password);

        // Send the message to server:
        send(socket_desc, password, strlen(password),0);

        // Receive the server's response:
        recv(socket_desc, server_message, sizeof(server_message),0);
        printf("%s",server_message);

        if (strcmp(server_message,"Account is blocked\n") == 0 || strcmp(server_message,"Account not ready\n") == 0){
            return 0;
        }

    } while (strcmp(server_message, "Account is signed in\n") != 0);
    
    memset(server_message, '\0', sizeof(server_message));
    //Change Password
    do {
        memset(new_password, '\0', sizeof(new_password));
        printf("Input your new password: ");
        gets(new_password);
        if ( new_password[0] == '\0' || strcmp(new_password,"bye") == 0){
            return -1;
        } else {
        // Send the message to server:
        send(socket_desc, new_password, strlen(new_password),0);
    
        // Receive the server's response:
        recv(socket_desc, server_message, sizeof(server_message),0);
        printf("%s\n",server_message);
        }    
    } while (strcmp(server_message,"Wrong input\n") != 0);

    // Close the socket:
    close(socket_desc);
    
    return 0;
}