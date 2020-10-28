#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000];
    char password[60], username[20];
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
        
        // Get input from the user:
        printf("Enter username: ");
        scanf("%s",username);
        fflush(stdin);
        if (strcmp(username,"") == 0 || strcmp(username,"bye") == 0){
            return 0;
        }

        // Send the message to server:
        send(socket_desc, username, strlen(username),0);
    
        // Receive the server's response:
        recv(socket_desc, server_message, sizeof(server_message),0);
        printf("%s\n",server_message);
    } while (strcmp(server_message,"Insert password\n") != 0);

    // CheckPassword
    do {
        password[0] = '\0';
        printf("Insert password\n");
        scanf("%s",password);
        fflush(stdin);

        // Send the message to server:
        send(socket_desc, password, strlen(password),0);

        // Receive the server's response:
        recv(socket_desc, server_message, sizeof(server_message),0);
        printf("%s\n",server_message);

        if (strcmp(server_message,"Account is blocked\n") == 0 || strcmp(server_message,"Account not ready\n") == 0){
            return 0;
        }

    } while (strcmp(server_message, "Account is signed in\n") != 0);
    
    //Change Password
    printf("Input your new password\n");
    scanf("%s",new_password);
    if (strcmp(new_password,"") == 0 || strcmp(new_password,"bye") == 0){
        return 0;
    } else {
        // Send the message to server:
        send(socket_desc, new_password, strlen(new_password),0);
    
        // Receive the server's response:
        recv(socket_desc, server_message, sizeof(server_message),0);
        printf("%s\n",server_message);
    }    


    // Close the socket:
    close(socket_desc);
    
    return 0;
}