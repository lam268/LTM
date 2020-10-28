#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], buffer[20];
    int server_struct_length = sizeof(server_addr);
    
    // catch wrong input
	// catch wrong input
	if(argc!=3){
		printf("Please input IP address and port number\n");
		return 0;
	}
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(buffer, '\0', sizeof(buffer));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Get input from the user:
    do {
        printf("Input username: ");
        scanf("%[^\n]",buffer);
        if (strcmp(buffer,"") == 0){
            return 0;
        }
    } while (strcmp(buffer,"") != 0);
    
    
    // Send the message to server:
    if(sendto(socket_desc, client_message, strlen(client_message), 0,
         (struct sockaddr*)&server_addr, server_struct_length) < 0){
        printf("Unable to send message\n");
        return -1;
    }
    
    // Receive the server's response:
    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
         (struct sockaddr*)&server_addr, &server_struct_length) < 0){
        printf("Error while receiving server's msg\n");
        return -1;
    }
    
    printf("Server's response: %s\n", server_message);
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}
