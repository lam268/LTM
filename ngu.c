// hello.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 1000

struct person {
    char username[20];
    char password[20];
    char status[2];
    int signin;
    struct person *next;
};
typedef struct person account;
account *first, *last;

void splitstr(char s[]) {
    int i = 0;
    char *ptr = strtok (s, " ");
    char *array[3];
    account *p;

    while (ptr != NULL)
    {
        array[i++] = ptr;
        ptr = strtok (NULL, " ");
    }
    p = (account *)malloc(sizeof(account));
    p->next = NULL;
    strcpy(p->username,array[0]);
    strcpy(p->password,array[1]);
    strcpy(p->status,array[2]);
    if (first == NULL){
        first = p;
        last = p;
    }
    else {
        last -> next = p;
        last = p;
    }
}

void printlist() {
    account *p;
    if (first == NULL){
        printf("Linkedlist is empty!");
    }
    else {
        p = first;
        int i = 0;
        while (p != NULL){
            i++;
            printf("%d %s %s %s\n",i,p->username,p->password,p->status);
            p = p->next;
        }
    }
}

void readfile(void) {
    FILE* file = fopen("D:\\LTM\\week1\\nguoidung.txt", "r");
    char line[256];
    account *p;
    if (file == NULL) {
        printf("Error in reading file!\n");
        exit(1);
    }
    while (fgets(line, MAXCHAR, file)) {
        splitstr(line);
    }
    printlist();
    fclose(file);
}

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
    printf("Your choice (1-6, other to quit): \n");
    scanf("%d", &n);
    return n;
}

void Register() {
    char username[20];
    char password[20];
    account *p;
    int found = 0;
    printf("Username: \n");
    scanf("%s", username);
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(username,p->username) == 0){
                found = 1;
                break;
            }
            else {
                p = p->next;
            }
        }
    }
    if (found == 1){
        printf("Account existed!!!\n");
    } else {
        printf("Password: \n");
        scanf("%s", password);
        p = (account *)malloc(sizeof(account));
        p->next = NULL;
        strcpy(p->username, username);
        strcpy(p->password, password);
        strcpy(p->status,"2");
        last -> next = p;
        last = p;
    }
    printlist();
}

void Activate() {
    char username[20];
    account *p;
    int found = 4;
    printf("Username: \n");
    scanf("%s", username);
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(username,p->username) == 0){
                found = atoi(p->status);
                break;
            }
            else {
                p = p->next;
            }
        }
    }
    if (found == 2) {
        char str[20];
        int count = 0;
        while (strcmp("LTM121216",str) != 0 && count < 4){
            printf("Account is not activated\n");
            printf("Input activation code: \n");
            scanf("%s",str);
            count++;
        }
        if (count > 3){
            strcpy(p->status,"0");
            printf("Activation code is incorrect.Account is blocked\n");
        } else {
            strcpy(p->status,"1");
            printf("Account is activated\n");
        }
    } else if (found == 1){
        printf("Account is activated\n");
    } else if (found == 0){
        printf("Account is blocked\n");
    }
    else {
        printf("This account is not registered.\n");
    }
}

void Signin(){
    char username[20];
    char password[20];
    account *p;
    int found = 4;
    printf("Username: \n");
    scanf("%s", username);
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(username,p->username) == 0){
                found = atoi(p->status);
                break;
            }
            else {
                p = p->next;
            }
        }
    }
    if (found == 1) {
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
            strcpy(p->status,"0");
            printf("Password is incorrect.Account is blocked\n");
        } else {
            printf("Hello %s\n",p->username);
        }
    } else if (found == 2){
        printf("Account is not activated\n");
        printf("Please activate ur account in option 2\n");
    } else if (found == 0){
        printf("Account is blocked\n");
    }
    else {
        printf("This account is not registered.\n");
    }
}

void Searchwhensignin() {
    account *p;
    int signin;
    char username[20];
    p = first;
    if ( p != NULL ){
        while ( p != NULL){
            if (p->signin == 1){
                signin = p ->signin;
                break;
            } else {
                p = p->next;
            }
        }
    }
    if (signin == 1){
        printf("Username\n");
        scanf("%s",username);
        int found =4;
        p = first;
        if (p != NULL) {
            while (p != NULL) {
                if (strcmp(username,p->username) == 0){
                    found = atoi(p->status);
                    break;
                }
                else {
                    p = p->next;
                }
            }
        }
        if (found == 1){
            printf("Account is active\n");
        } else if (found == 0){
            printf("Account is blocked\n");
        } else if (found == 2){
            printf("Account is not activated\n");
        } else {
            printf("Cannot find account\n");
        }
    }   
}

void ChangePassword() {
    char username[20];
    char password[20];
    account *p;
    int found = 4;
    printf("Username \n");
    scanf("%s", username);
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(username,p->username) == 0){
                found = atoi(p->status);
                break;
            }
            else {
                p = p->next;
            }
        }
    }
    if (found == 0){
        printf("Account is blocked\n");
    } else if (found == 2) {
        printf("Account is not activated\n");
    } else if (found == 1) {
        if (p->signin == 1){
            printf("Password\n");
            scanf("%s",password);
        }
        while (strcmp(password,p->password) != 0){
            printf("Password\n");
            scanf("%s",password);
        }
        char np[20];
        printf("New Password\n");
        scanf("%s",np);
        strcmp(p->password,np);
    } else {
        printf("Cannot find account\n");
    }
}

void Signout() {
    char username[20];
    account *p;
    int found = 4;
    printf("Username: \n");
    scanf("%s", username);
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(username,p->username) == 0){
                found = atoi(p->status);
                break;
            }
            else {
                p = p->next;
            }
        }
    }
    if(p->signin == 1){
        p->signin = 0;
        printf("Goodbye %s\n",p->username);
    } else if (found == 4) {
        printf("Cannot find account\n");
    }  
}

void OutputFile(){
    FILE* file = fopen("D:\\LTM\\week1\\nguoidung.txt", "w");
    account *p;
    if (file == NULL) {
        printf("Error in reading file!\n");
        exit(1);
    }
    p = first;
    while (p != NULL) {
        fprintf(file,"%s %s %s\n",p->username,p->password,p->status);
        p = p->next;
    }
    fclose(file);
}

int main() {
    int n,m;
    readfile();
    do 
    {
        n = printmenu();
        switch (n) {
            case 1: 
            Register();
            break;
            case 2: 
            Activate();
            break;
            case 3: 
            Signin();
            break;
            case 4:
            Searchwhensignin();
            break;
            case 5: 
            ChangePassword();
            break;
            case 6:
            Signout(); 
            break;
            default:
            printf("Goodbye!!!");
            m = 1; 
            break;
        }
    }
    while (m != 1);
    OutputFile();
}