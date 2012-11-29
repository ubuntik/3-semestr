#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void main()
{
	int sockfd, newsockfd;
	int clilen;
	int n;
	char line[1000];

	struct sockaddr_in servaddr, cliaddr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror(NULL);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family= AF_INET;
	servaddr.sin_port= htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	if(listen(sockfd, 5) < 0){
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	while(1){
		clilen = sizeof(cliaddr);
		if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0){
			perror(NULL);
			close(sockfd);
			exit(1);
		}

		while((n = read(newsockfd, line, 999)) > 0){
			if((n = write(newsockfd, line, strlen(line)+1)) < 0){
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}
		}

	if(n < 0){
			perror(NULL);
			close(sockfd);
			close(newsockfd);
			exit(1);
		}

		close(newsockfd);
	}
}

