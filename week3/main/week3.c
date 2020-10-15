#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/lib.h"

#define MAXCHAR 1000

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
    FILE* file = fopen("D:\\LTM\\LTM\\week3\\nguoidung.txt", "r");
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

void Register() {
    char username[20];
    char password[20];
    char homepage[60];
    account *p;
    printf("Username\n");
    scanf("%s", username);
    p = CheckAccount(username);
    if (p != NULL) {
        printf("Account existed!!!\n");
    } else {
        printf("Password: \n");
        scanf("%s", password);
        printf("Homepage: \n");
        scanf("%s", homepage);
        p = (account *)malloc(sizeof(account));
        p->next = NULL;
        strcpy(p->username, username);
        strcpy(p->password, password);
        strcpy(p->homepage, homepage);
        p->status = 2;
        last -> next = p;
        last = p;
    }
    current = NULL;
}

void Activate() {
    char username[20];
    account *p;
    printf("Username\n");
    scanf("%s", username);
    p = CheckAccount(username);
    if(p->status == 1){
        printf("Account is activated\n");
    } else if (p->status == 2) {
        char str[20];
        int count = 0;
        while (strcmp("LTM121216",str) != 0 && count < 4){
            printf("Account is not activated\n");
            printf("Input activation code: \n");
            scanf("%s",str);
            count++;
        }
        if (count > 3){
            p->status = 0;
            printf("Activation code is incorrect.Account is blocked\n");
        } else {
            p->status = 1;
            printf("Account is activated\n");
        }
    } else if (p->status == 0) {
        printf("Account is blocked\n");
    } else {
        printf("Cannot find this account\n");
    }
    current = NULL;
}

void OutputFile(){
    FILE* file = fopen("D:\\LTM\\LTM\\week3\\nguoidung.txt", "w");
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
    FILE* file = fopen("D:\\LTM\\LTM\\week3\\nguoidung.txt", "w");
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

void Search() {
    char s[20];
    char username[20];
    account *p;
    printf("Username:\n");
    scanf("%s",s);
    p = CheckAccount(s);
    if (p->signin == 1){
        printf("Username\n");
        scanf("%s",username);
        p = CheckAccount(username);
        if (p->status == 1){
            printf("Account is active\n");
        } else if (p->status == 0){
            printf("Account is blocked\n");
        } else if (p->status == 2){
            printf("Account is not activated\n");
        } else {
            printf("Cannot find account\n");
        }
    } else {
        printf("You are not signed in \n");
    }
    current = NULL;
}

void ChangePassword() {
    char s[20];
    char username[20];
    char password[20];
    account *p;
    char new_password[20];
    printf("Username:\n");
    scanf("%s",username);
    p = CheckAccount(username);
    if (p->signin == 1){
        if (p->status == 1){
            int count = 0;
            printf("Old password\n");
            scanf("%s",password);
            while (strcmp(p->password,password) != 0 && count < 4){
                printf("Old password\n");
                scanf("%s",password);
                count++;
            }
            if (count > 3){
                p->status = 0;
                printf("Account is blocked\n");
            }
                printf("New password\n");
                scanf("%s",new_password);
                strcpy(p->password,new_password); 
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

void printfdomains() {
    char s[20];
    account *p;
    char domain[60];
    printf("Username:\n");
    scanf("%s",s);
    p = CheckAccount(s);
    if (p->signin == 1){
        printf("%s\n", p->homepage);
        get_ip(p->homepage);
    } else {
        printf("You are not signed in \n");
    }
    current = NULL;
}

void printfIP() {
    char s[20];
    char ip[20];
    account *p;
    printf("Username:\n");
    scanf("%s",s);
    p = CheckAccount(s);
    if (p->signin == 1){
        get_domain(p->homepage);
    } else {
        printf("You are not signed in \n");
    }
}

int main() {
    int n,m;
    readFile();
    do 
    {
        n = printmenu();
        switch (n) {
            case 1: 
            Register();
            OutputFile();
            break;
            case 2: 
            Activate();
            OutputFile();
            break;
            case 3: 
            Signin();
            OutputFile();
            break;
            case 4:
            Search();
            OutputFile();
            break;
            case 5: 
            ChangePassword();
            OutputFile();
            break;
            case 6:
            Signout();
            OutputFile();
            break;
            case 7:
            printfdomains();
            OutputFile();
            break;
            case 8:
            printfIP();
            OutputFile();
            break;
            default:
            printf("Goodbye!!!");
            m = 1; 
            break;
        }
    }
    while (m != 1);
    OutputFileLast();
}