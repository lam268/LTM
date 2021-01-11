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
#define MAX_CLIENT 10

struct person
{
    char username[20];
    char password[20];
    int status;
    int signin;
    struct person *next;
};
typedef struct person account;
account *first, *last, *current;

void splitstr(char *s)
{
    int i = 0;
    char *token = strtok(s, " ");
    char *array[3];
    account *p;
    while (token != NULL)
    {
        array[i++] = token;
        token = strtok(NULL, " ");
    }
    p = (account *)malloc(sizeof(account));
    p->next = NULL;
    strcpy(p->username, array[0]);
    strcpy(p->password, array[1]);
    p->status = atoi(array[2]);
    p->signin = 0;
    if (first == NULL)
    {
        first = p;
        last = p;
    }
    else
    {
        last->next = p;
        last = p;
    }
}

void readFile()
{
    FILE *file = fopen("D:\\LTM\\LTM\\week6\\nguoidung.txt", "r");
    char line[256];
    account *p;
    if (file == NULL)
    {
        printf("Error in reading file!\n");
        exit(1);
    }
    while (fgets(line, MAXCHAR, file))
    {
        splitstr(line);
    }
    fclose(file);
}

void OutputFile()
{
    FILE *file = fopen("D:\\LTM\\LTM\\week6\\nguoidung.txt", "w");
    account *p;
    if (file == NULL)
    {
        printf("Error in reading file!\n");
        exit(1);
    }
    p = first;
    while (p != NULL)
    {
        fprintf(file, "%s %s %d\n", p->username, p->password, p->status);
        p = p->next;
    }
    fclose(file);
}

account *CheckAccount(char *s)
{
    account *p;
    p = first;
    if (p != NULL)
    {
        while (p != NULL)
        {
            if (strcmp(s, p->username) == 0)
            {
                current = p;
                break;
            }
            else
            {
                p = p->next;
            }
        }
    }
    return current;
}

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, client_size, socket_temp;
    struct sockaddr_in server_addr, client_addr;
    char server_message[100], client_message[100];
    char username[20], password[20], new_password[20];
    char only_string[20], only_number[20];
    fd_set rset;
    struct timeval tv;
    int maxfdp, nready;
    account *p;
    int opt = 1;
    int client_num[10];

    readFile();
    if (argc == 1)
    {
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

    for (int j = 0; j < MAX_CLIENT; j++)
    {
        client_num[j] = 0;
    }

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        perror("Error while creating socket\n");
        return EXIT_FAILURE;
    }
    printf("Socket created successfully\n");

    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Error in setsockopt\n");
        exit(EXIT_FAILURE);
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return EXIT_FAILURE;
    }
    printf("Done with binding\n");

    // Listen for clients:
    if (listen(socket_desc, 3) < 0)
    {
        printf("Error while listening\n");
        return EXIT_FAILURE;
    }
    printf("\nListening for incoming connections.....\n");

    while (1)
    {
        FD_ZERO(&rset);
        FD_SET(socket_desc, &rset);
        maxfdp = socket_desc;
        for (int i = 0; i < MAX_CLIENT; i++)
        {
            socket_temp = client_num[i];

            if (socket_temp > 0)
            {
                FD_SET(socket_temp, &rset);
            }
            if (socket_temp > maxfdp)
            {
                maxfdp = socket_temp;
            }
        }
        nready = select(maxfdp + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(socket_desc, &rset))
        {
            // Accept an incoming connection:
            client_size = sizeof(client_addr);
            client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);
            if (client_sock < 0)
            {
                printf("Cant accept\n");
                return EXIT_FAILURE;
            }
            printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            for (int i = 0; i < 10; i++)
            {
                if (client_num[i] == 0)
                {
                    client_num[i] = client_sock;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENT; i++)
        {
            socket_temp = client_num[i];
            int client_now = i;
            if (FD_ISSET(socket_temp, &rset))
            {
                // Check Account:
                do
                {
                    memset(server_message, '\0', sizeof(server_message));
                    memset(username, '\0', sizeof(username));
                    if (recv(client_sock, username, sizeof(username), 0) < 0)
                    {
                        printf("Couldn't receive\n");
                        return EXIT_FAILURE;
                    }
                    printf("Username: %s\n", username);
                    if (username[0] == '\0' || strcmp(username, "bye") == 0)
                    {
                        printf("Couldn't receive\n");
                        return EXIT_FAILURE;
                    }
                    p = CheckAccount(username);
                    if (p != NULL)
                    {
                        strcpy(server_message, "Insert password\n");
                        send(client_sock, server_message, strlen(server_message), 0);
                        break;
                    }
                    else
                    {
                        strcpy(server_message, "Wrong account\n");
                        send(client_sock, server_message, strlen(server_message), 0);
                    }
                } while (strcmp(username, "") != 0);
                int count = 0;

                // Check Password
                memset(server_message, '\0', sizeof(server_message));
                do
                {
                    memset(password, '\0', sizeof(password));
                    if (recv(client_sock, password, sizeof(password), 0) < 0)
                    {
                        printf("Couldn't receive\n");
                        return EXIT_FAILURE;
                    }
                    printf("Password: %s\n", password);
                    if (strcmp(password, p->password) != 0)
                    {
                        if (count > 3)
                        {
                            p->status = 0;
                            OutputFile();
                            strcpy(server_message, "Account is blocked\n");
                            send(client_sock, server_message, strlen(server_message), 0);
                            return EXIT_FAILURE;
                        }
                        strcpy(server_message, "Wrong password\n");
                        send(client_sock, server_message, strlen(server_message), 0);
                        count++;
                    }
                    else
                    {
                        if (p->status != 1)
                        {
                            strcpy(server_message, "Account not ready\n");
                            send(client_sock, server_message, strlen(server_message), 0);
                            return EXIT_FAILURE;
                        }
                        strcpy(server_message, "Account is signed in\n");
                        send(client_sock, server_message, strlen(server_message), 0);
                        p->signin = 1;
                        OutputFile();
                        break;
                    }
                } while (true);
                // Change Password
                do
                {
                    memset(server_message, '\0', sizeof(server_message));
                    int error = 1;
                    if (recv(client_sock, new_password, sizeof(new_password), 0) < 0)
                    {
                        printf("Couldn't receive\n");
                        return EXIT_FAILURE;
                    }
                    int j, k = 0;
                    printf("New Password: %s\n", new_password);
                    for (int i = 0; i < strlen(new_password); i++)
                    {
                        if (isdigit(new_password[i]) != 0)
                        {
                            only_number[j] = new_password[i];
                            j++;
                        }
                        else if (isalpha(new_password[i]) != 0)
                        {
                            only_string[k] = new_password[i];
                            k++;
                        }
                        else
                        {
                            error = 0;
                        }
                    }
                    only_number[j] = '\0';
                    only_string[k] = '\0';
                    char temp[20];
                    strcpy(temp, only_number);
                    strcat(temp, only_string);
                    if (error == 0)
                    {
                        strcpy(server_message, "Wrong input\n");
                        send(client_sock, server_message, strlen(server_message), 0);
                        break;
                    }
                    else
                    {
                        strcpy(p->password, new_password);
                        OutputFile();
                        strcpy(server_message, temp);
                        send(client_sock, server_message, strlen(server_message), 0);
                    }
                } while (true);
            }
        }
    }
    close(socket_desc);
    return 0;
}