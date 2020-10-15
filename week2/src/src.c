#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

struct hostent *host;
struct in_addr addr;
struct in_addr **addr_list;
struct sockaddr_in sk;

void get_ip(char *ip) {
    if(!(inet_pton(AF_INET, ip, &(sk.sin_addr)) != 0)){
            printf("Wrong parameter\n");
    }    
    inet_aton(ip,&addr);
    host = gethostbyaddr(&addr,sizeof(addr),AF_INET);
    if(host) {
        printf("Official name: %s\n",host->h_name);
        if(host->h_aliases[0] != NULL) {
            printf("Alias name:\n");
            for(int i = 0; host->h_aliases[i] != NULL; i++) {
            printf("\t%s\n", host->h_aliases[i]);}
        }
    } else {
        printf("Not found information\n");
    }   
}

void get_domain(char *domain) {
    int i = 0;
    if((inet_pton(AF_INET, domain, &(sk.sin_addr)) != 0)){
            printf("Wrong parameter\n");
    }  
    host = gethostbyname(domain);
    addr_list = (struct in_addr **)host->h_addr_list;
    if(host){
        for(i = 0; addr_list[i] != NULL; i++) {
            if (i==0) {
                printf("Official IP: %s \n", inet_ntoa(*addr_list[i]));
                printf("Alias IP: \n");
            }
            else{    
                printf("%s \n", inet_ntoa(*addr_list[i]));
            }
        }
    }
    else{
        printf("Not found information\n");
    }
}

