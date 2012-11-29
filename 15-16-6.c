/* A simple echo UNIX Domain datagram client */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
	int sockfd;
	int n, len;
	char sendline[1000], recvline[1000];

	struct sockaddr_un servaddr, cliaddr;

	if((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) 
	{
		perror(NULL);
		exit(1);
	}

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sun_family= AF_UNIX;
	strcpy(cliaddr.sun_path,"AAAA");

	if(bind(sockfd, (struct sockaddr *) &cliaddr, SUN_LEN(&cliaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path,"BBBB");
	printf("String => ");
	fgets(sendline, 1000, stdin);

	if(sendto(sockfd, sendline, strlen(sendline)+1, 0, (struct sockaddr *) &servaddr, SUN_LEN(&servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	if((n = recvfrom(sockfd, recvline, 1000, 0, (struct sockaddr *) NULL, NULL)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	recvline[n] = 0;
	printf("%s", recvline);

	close(sockfd);
	return 0;
}

