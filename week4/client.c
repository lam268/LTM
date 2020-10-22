#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#define MAXLINE 1000 

// Driver code 
int main(int argc, char *argv[]) 
{
	// catch wrong input
	if(argc!=3){
		printf("Please input IP address and port number\n");
		exit(0);
	}
	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);

	int sockfd, n; 
	struct sockaddr_in servaddr; 
	
	// clear servaddr 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_addr.s_addr = inet_addr(ip_address); 
	servaddr.sin_port = htons(port); 
	servaddr.sin_family = AF_INET; 
		
	// create datagram socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	// connect to server 
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	{ 
		printf("\n Error : Connect Failed \n"); 
		exit(0); 
	} 
	char username[20];
	int len;
	char buffer[100];
	do {
		// get message
		printf("input username: \n"); 
		scanf("%[^\n]",&username);
		if (username[0] == '\0') {
			exit(0);
		}
		sendto(sockfd, username, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
		recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL); 
		printf("OUTPUT: ");
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    	buffer[n] = '\0'; 
    	printf("%s\n", buffer);
		puts("-------------------------");
	}while(username[0] != '\0');	
	close(sockfd); 
}