#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int sockfd;
	int n, len;
	char sendline[1000], recvline[1000];

	struct sockaddr_in servaddr, cliaddr;

	if(argc != 2){
		printf("Usage: a.out <IP address>\n");
		exit(1);
	}

	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
		perror(NULL);
		exit(1);
	}

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7);

	if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	printf("String => ");

	fgets(sendline, 1000, stdin);

	if(sendto(sockfd, sendline, strlen(sendline)+1, 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	if((n = recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr *) NULL, NULL)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	printf("%s\n", recvline);

	close(sockfd);
	return 0;
}

