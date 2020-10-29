#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include<unistd.h>
#include <arpa/inet.h>
#define MAXCHAR 1000


struct person {
    char username[20];
    char password[20];
    int status;
    int signin;
    struct person *next;
};
typedef struct person account;
account *first, *last, *current;

void splitstr(char *s) {
    int i = 0;
    char *token = strtok(s," ");
    char *array[3];
    account *p;
    while (token != NULL) {
        array[i++] = token;
        token = strtok(NULL," ");
    }
    p = (account *)malloc(sizeof(account));
    p->next = NULL;
    strcpy(p->username,array[0]);
    strcpy(p->password,array[1]);
    p -> status = atoi(array[2]);
    p->signin = 0;
    if (first == NULL){
        first = p;
        last = p;
    } else {
        last -> next = p;
        last = p;
    }
}

void readFile(){
    FILE* file = fopen("D:\\LTM\\LTM\\week5\\nguoidung.txt", "r");
    char line[256];
    account *p;
    if (file == NULL) {
        printf("Error in reading file!\n");
        exit(1);
    }
    while (fgets(line, MAXCHAR, file)) {
        splitstr(line);
    }
    fclose(file);
}

void OutputFile(){
    FILE* file = fopen("D:\\LTM\\LTM\\week5\\nguoidung.txt", "w");
    account *p;
    if (file == NULL) {
        printf("Error in reading file!\n");
        exit(1);
    }
    p = first;
    while (p != NULL) {
        fprintf(file,"%s %s %d\n",p->username,p->password,p->status);
        p = p->next;
    }
    fclose(file);
}

account *CheckAccount(char *s) {
    account *p;
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(s, p->username) == 0){
                current = p;
                break;
            }
            else {
                p = p -> next;
            }
        }
    }
    return current;
}

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[100], client_message[100];
    char username[20], password[20], new_password[20];
    char only_string[20], only_number[20];
    readFile();
    if(argc == 1){
        printf("Please input port number!!!\n");
        return 0;
    }

    int port = atoi(argv[1]);

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    memset(new_password, '\0', sizeof(new_password));
    memset(only_number, '\0', sizeof(only_number));
    memset(only_string, '\0', sizeof(only_string));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    
    // Listen for clients:
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_sock < 0){
        printf("Can't accept\n");
        return -1;
    }

    account *p;

    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    // Check Account:
    do {
        memset(server_message, '\0', sizeof(server_message));
        memset(username, '\0', sizeof(username));
        if (recv(client_sock, username, sizeof(username), 0) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        printf("Username: %s\n", username);
        if (username[0] == '\0' || strcmp(username,"bye") == 0){
            printf("Couldn't receive\n");
            return -1;
        }
        p = CheckAccount(username);
        if (p != NULL){
            strcpy(server_message, "Insert password\n");
            send(client_sock, server_message, strlen(server_message), 0);
            break;
        } else {
            strcpy(server_message, "Wrong account\n");
            send(client_sock, server_message, strlen(server_message), 0);
        }
    } while (strcmp(username,"") != 0);
    int count = 0;
    
    // Check Password
    memset(server_message, '\0', sizeof(server_message));
    do {
        memset(password, '\0', sizeof(password));
        if (recv(client_sock, password, sizeof(password), 0) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        printf("Password: %s\n", password);
        if (strcmp(password,p->password) != 0){
            if (count > 3){
                p->status = 0;
                OutputFile();
                strcpy(server_message,"Account is blocked\n");
                send(client_sock, server_message, strlen(server_message), 0);
                return 0;
            } 
            strcpy(server_message, "Wrong password\n");
            send(client_sock, server_message, strlen(server_message), 0);
            count++;
        } else {
            if (p->status != 1){
                strcpy(server_message, "Account not ready\n");
                send(client_sock, server_message, strlen(server_message), 0);
                return 0;
            }
            strcpy(server_message,"Account is signed in\n");
            send(client_sock, server_message, strlen(server_message), 0);
            p->signin = 1;
            OutputFile();
            break;
        }
    } while (true);    
    // Change Password
    do {
        memset(server_message, '\0', sizeof(server_message));
        int error = 1;
        if (recv(client_sock, new_password, sizeof(new_password), 0) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        int j,k = 0;
        printf("New Password: %s\n", new_password);
        for (int i = 0; i < strlen(new_password); i ++){
            if (isdigit(new_password[i]) != 0){
                only_number[j] = new_password[i];
                j++;
            } else if (isalpha(new_password[i]) != 0){
                only_string[k] = new_password[i];
                k++;
            } else {
                error = 0;
            }
        }
        only_number[j] = '\0';
        only_string[k] = '\0';
        char temp[20];
        strcpy(temp,only_number);
        strcat(temp,only_string);
        if (error == 0){
            strcpy(server_message, "Wrong input\n");
            send(client_sock, server_message, strlen(server_message), 0);
            break;
        } else {
            strcpy(p->password,new_password);
            OutputFile();
            strcpy(server_message, temp);
            send(client_sock, server_message, strlen(server_message), 0);
        }
    } while (true);
       
    // Closing the socket:
    close(client_sock);
    close(socket_desc);
    
    return 0;
}