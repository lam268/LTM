#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
#define MAXCHAR 1000 
#define MAXLINE 1000

struct person {
    char username[20];
    char password[20];
    char homepage[60];
    int status;
    int signin;
    struct person *next;
};
typedef struct person account;
account *first, *last, *current;

int printmenu() {
    int n;
    printf("USER MANAGEMENT PROGRAM\n");
    printf("-----------------------------------\n");
    printf("1. Register\n");
    printf("2. Activate\n");
    printf("3. Sign in\n");
    printf("4. Search\n");
    printf("5. Change password\n");
    printf("6. Sign out\n");
    printf("7. Homepage with domain name\n");
    printf("8. Homepage with IP address\n");
    printf("Your choice (1-6, other to quit): \n");
    scanf("%d", &n);
    return n;
}

void splitstr(char *s) {
    int i = 0;
    char *token = strtok(s," ");
    char *array[5];
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
    strcpy(p->homepage,array[3]);
    p -> signin = atoi(array[4]);
    if (first == NULL){
        first = p;
        last = p;
    } else {
        last -> next = p;
        last = p;
    }
}

void printlist() {
    account *p;
    p = first;
    while ( p != NULL){
        printf("%s %s %d %d\n",p->username,p->password,p->status,p->signin);
        p = p->next;
    }
}

void readFile(){
    FILE* file = fopen("D:\\LTM\\LTM\\week4\\nguoidung.txt", "r");
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

void OutputFile(){
    FILE* file = fopen("D:\\LTM\\LTM\\week4\\nguoidung.txt", "w");
    account *p;
    if (file == NULL) {
        printf("Error in reading file!\n");
        exit(1);
    }
    p = first;
    while (p != NULL) {
        fprintf(file,"%s %s %d %s %d\n",p->username,p->password,p->status,p->homepage,p->signin);
        p = p->next;
    }
    fclose(file);
}

void OutputFileLast(){
    FILE* file = fopen("D:\\LTM\\LTM\\week4\\nguoidung.txt", "w");
    account *p;
    if (file == NULL) {
        printf("Error in reading file!\n");
        exit(1);
    }
    p = first;
    while (p != NULL) {
        p->signin = 0;
        fprintf(file,"%s %s %d %s %d\n",p->username,p->password,p->status,p->homepage,p->signin);
        p = p->next;
    }
    fclose(file);
}

void Signin() {
    char username[20];
    char password[20];
    account *p;
    printf("Username: \n");
    scanf("%s", username);
    p = CheckAccount(username);
    if (p->status == 1) {
        char str[20];
        int count = 0;
        printf("Password \n");
        scanf("%s", str);
        while((strcmp(str,p->password) != 0) && count < 4){
            printf("Wrong password.\n");
            printf("Password \n");
            scanf("%s", str);
            count++;
        }
        if (count > 3){
            p->status = 0;
            printf("Password is incorrect.Account is blocked\n");
        } else {
            printf("Hello %s\n",p->username);
            p->signin = 1;
        }
    } else if (p->status == 2){
        printf("Account is not activated\n");
        printf("Please activate ur account in option 2\n");
    } else if (p->status == 0){
        printf("Account is blocked\n");
    }
    else {
        printf("This account is not registered.\n");
    }
    current =NULL;
}

void Signout() {
    char username[20];
    account *p;
    printf("Username:\n");
    scanf("%s",username);
    p = CheckAccount(username);
    if (p -> signin == 1){
        if (p->status == 1){
            p -> signin = 0;
            printf("Goodbye %s\n",p->username);
        } else if (p->status == 0){
            printf("Account is blocked\n");
        } else {
            printf("Cannot find account\n");
        }
    } else {
        printf("You are not signed in \n");
    }
    current = NULL;
}

int main(int argc, char *argv[]) {
    if (argc == 1){
        printf("Please input port number!");
        return 0;
    }
    int port_number = atoi(argv[1]);
    char buffer1[100];
    char buffer2[100];
    char user_name[100];
    char password[100];
    int listenfd, len1,len2; 
	struct sockaddr_in servaddr, cliaddr1, cliaddr2; 
	bzero(&servaddr, sizeof(servaddr)); 
	// Create a UDP Socket 
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);		 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port_number); 
	servaddr.sin_family = AF_INET; 

	// bind server address to socket descriptor 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    while(1){
	//receive the datagram from client 1 and 2
	len1 = sizeof(cliaddr1); 
	len2 = sizeof(cliaddr2); 
	int n1 = recvfrom(listenfd, buffer1, sizeof(buffer1), 0, (struct sockaddr*)&cliaddr1,&len1);
	int n2 = recvfrom(listenfd, buffer2, sizeof(buffer2), 0, (struct sockaddr*)&cliaddr2,&len2);  

	buffer1[n1] = '\0';
	buffer2[n2] = '\0';
    Signin();
    }
}