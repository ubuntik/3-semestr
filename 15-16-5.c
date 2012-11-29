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
	int clilen, n;
	char line[1000];

	struct sockaddr_un servaddr, cliaddr;

	if((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) 
	{
		perror(NULL);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path,"BBBB");

	if(bind(sockfd, (struct sockaddr *) &servaddr, SUN_LEN(&servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	while(1) {
		clilen = sizeof(struct sockaddr_un);
		if((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *) &cliaddr, &clilen)) < 0){
			perror(NULL);
			close(sockfd);
			exit(1);
		}

		if(sendto(sockfd, line, strlen(line), 0, (struct sockaddr *) &cliaddr, clilen) < 0){
			perror(NULL);
			close(sockfd);
			exit(1);
		}
	}

	return 0;
}

