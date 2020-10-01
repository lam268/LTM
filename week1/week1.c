#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 1000

struct person {
    char username[20];
    char password[20];
    int status;
    int signin;
    struct person *next;
};
typedef struct person account;
account *first, *last;

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
    p -> signin = 0;
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
    fclose(file);
}

void Register() {
    char username[20];
    char password[20];
    account *p;
    int found;
    printf("Username\n");
    scanf("%s", username);
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(username, p->username) == 0){
                found = 1;
                break;
            }
            else {
                p = p -> next;
            }
        }
    }
    if (found == 1) {
        printf("Account existed!!!\n");
    } else {
        printf("Password: \n");
        scanf("%s", password);
        p = (account *)malloc(sizeof(account));
        p->next = NULL;
        strcpy(p->username, username);
        strcpy(p->password, password);
        p->status = 2;
        last -> next = p;
        last = p;
    }
}

void Activate() {
    char username[20];
    account *p;
    int found = 4;
    printf("Username\n");
    scanf("%s", username);
    p = first;
    if (p != NULL) {
        while (p != NULL) {
            if (strcmp(username, p->username) == 0){
                found = p -> status;
                break;
            }
            else {
                p = p -> next;
            }
        }
    }
    if( found == 1){
        printf("Account is activated\n");
    } else if (found == 2) {
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
    } else if (found == 0) {
        printf("Account is blocked\n");
    } else {
        printf("Cannot find this account\n");
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
        fprintf(file,"%s %s %d\n",p->username,p->password,p->status);
        p = p->next;
    }
    fclose(file);
}

void Signin() {
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
                found = p -> status;
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
            p->status = 0;
            printf("Password is incorrect.Account is blocked\n");
        } else {
            printf("Hello %s\n",p->username);
            p->signin = 1;
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

int checkSignin() {
    account *p;
    p = first;
    int found = 0;
    if (p != NULL) {
        while (p != NULL) {
            if (p ->signin == 1){
                found = p -> signin;
                break;
            }
            else {
                p = p->next;
            }
        }
    }
    return found;
}

void Search() {
    int i = checkSignin();
    char username[20];
    account *p;
    if (i == 1){
        printf("Username\n");
        scanf("%s",username);
        int found =4;
        p = first;
        if (p != NULL) {
            while (p != NULL) {
                if (strcmp(username,p->username) == 0){
                    found = p->status;
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
    } else {
        printf("You are not signed in \n");
    }
}

void ChangePassword() {
    int i = checkSignin();
    char username[20];
    char password[20];
    char new_password[20];
    account *p;
    if (i == 1){
        printf("Username\n");
        scanf("%s",username);
        int found =4;
        p = first;
        if (p != NULL) {
            while (p != NULL) {
                if (strcmp(username,p->username) == 0){
                    found = p->status;
                    break;
                }
                else {
                    p = p->next;
                }
            }
        }
        if (found == 1){
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
        } else if (found == 0){
            printf("Account is blocked\n");
        } else {
            printf("Cannot find account\n");
        }
    } else {
        printf("You are not signed in \n");
    }
}

void Signout() {
    int i = checkSignin();
    char username[20];
    account *p;
    if (i == 1){
        printf("Username\n");
        scanf("%s",username);
        int found =4;
        p = first;
        if (p != NULL) {
            while (p != NULL) {
                if (strcmp(username,p->username) == 0){
                    found = p->status;
                    break;
                }
                else {
                    p = p->next;
                }
            }
        }
        if (found == 1){
            p -> signin = 0;
            printf("Goodbye %s\n",p->username);
        } else if (found == 0){
            printf("Account is blocked\n");
        } else {
            printf("Cannot find account\n");
        }
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
            break;
            case 2: 
            Activate();
            break;
            case 3: 
            Signin();
            break;
            case 4:
            Search();
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