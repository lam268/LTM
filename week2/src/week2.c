#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

struct hostent *host;
struct in_addr add;
struct sockaddr_in sk;
struct in_addr **list;


void get_ip(char *ip) {
    int i;
    if(!(inet_pton(AF_INET, ip, &(sk.sin_addr)) != 0)){
            printf("Wrong parameter\n");
    }    
    inet_aton(ip,&add);
    host = gethostbyaddr(&add,sizeof(add),AF_INET);
    if(host) {
        printf("Official name: %s\n",host->h_name);
        if(host->h_aliases[0] != NULL) {
            printf("Alias name:\n");
            for(i = 0; host->h_aliases[i] != NULL; i++) {
            printf("\t%s\n", host->h_aliases[i]);}
        }
        } else {
            printf("Not found information\n");
        }   
}

void get_domain(char *domain) {
    int i = 0;
    list = (struct in_addr **) host->h_addr_list;
    if((inet_pton(AF_INET, domain, &(sk.sin_addr)) != 0)){
            printf("Wrong parameter\n");
        }  
        host = gethostbyname(domain);
        if(host){
            printf("Official : %s \n", inet_ntoa(*list[i]));
            if(host->h_addr_list[0] != NULL)
            {
                printf("Alias IP:\n");
                i = 0;
                while(host->h_addr_list[i] != NULL){
                printf("%s ", inet_ntoa(*list[i]));
                }
            }
        }
        else{
            printf("Not found information\n");
        }
}

