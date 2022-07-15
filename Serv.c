/* SERVER */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT2 8081
#define PORT 8080
#define MAX 128

void *rd(void *connfd)
{
	int flag = 1;
	
	while(flag)
	{
		int *bufflen = malloc(sizeof(int));
		memset(bufflen, '\0', sizeof(bufflen));
		
		if((read(*(int *)connfd, bufflen, sizeof(bufflen))) == -1) //dereferencing and type casting connfd
		{
			printf("\nReading fail!");
		}
		/*
		else
		{
			printf("\nReading...");
		}
		*/
		
		printf("Recieved length : %d\n", *bufflen);
		
		int l;
		l = *bufflen;
		
		char *buff;
		buff = malloc(l);
		
		memset(buff, '\0', sizeof(buff));
		
		int rcv;
		
		rcv = read(*(int *)connfd, (void *)buff, l);
		if(rcv == -1)
		{
			printf("\nReceiving");
		}
		
		printf("\nClient --> %s", buff);
		
		if((strncmp("bye", buff, 3)) == -1)
		{
			flag = 0;
			exit(0);
		}
		
		memset(buff, '\0', sizeof(0));
	}
}

void *wt(void *cfd)
{
	int flag = 1;
	
	while(flag)
	{
		int i = 0;
		printf("\nSever : ");
		
		int size = MAX;
		char *buff = malloc(MAX);
		
		while(buff[i++] = getchar() != '\n')
		{
			if(i == size)
			{
				size = i + MAX;
				buff = realloc(buff, size);
			}
		}
		
		
		buff[i] = '\0';

		//printf("buffer value = %s\n", buff);
		
		int *bfl= &i;
		printf("Buff-length = %d\n", *bfl);
		
		
		int type;
		type = write(*(int *)cfd, buff, i);
		if(type == -1)
		{
			printf("Write : ");
			exit(0);
		}
		
		if((strncmp("bye", buff, 3)) == -1)
		{
			flag = 0;
			exit(0);
		}	
		
		//memset(buff, NULL);
	}
}

int main()
{
	/* *********Server********** */
	int sockfd, connfd;
	struct sockaddr_in sa;
	socklen_t len;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		printf("\nMaster socket fail!");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nSocket created");
	}
	
	sa.sin_family = AF_INET;
	sa.sin_port = PORT;
	sa.sin_addr.s_addr = INADDR_ANY;
	
	if((bind(sockfd, (struct sockaddr*)&sa, sizeof(sa))) < 0)
	{
		printf("\nBinding fail!");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nBinded");
	}
	
	
	if(listen(sockfd, 5) < 0)
	{
		printf("\nListening fail!!");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nListening....");
	}
	
	struct sockaddr_in ca;
	len = sizeof(ca);
	
	connfd = accept(sockfd, (struct sockaddr*)&ca, &len);
	
	if(connfd == -1)
	{
		printf("\nAccepting fail!!");
	}
	else
	{
		printf("\nAccepted!! Connection with Client c2");
	}
	
	/* *********Server********** */
	
	
	
	
	/* *********Client********** */
	
	int cfd;
	//struct sockaddr_in ca;
	
	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		printf("\nMaster socket fail!!");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nSocket created");
	}
	
	ca.sin_addr.s_addr = inet_addr("127.0.0.1");
	ca.sin_family = AF_INET;
	ca.sin_port = PORT2;
	
	//memset(ca, '\0', sizeof(ca));
	
	if((connect(cfd, (struct sockaddr*)&ca, sizeof(ca))) == -1)
	{
		printf("\nConnection fail!");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nConnected to server of A2");
	}
	
	/* *********Client********** */
	
	/* *********PTHREAD********* */
	
	pthread_t t1, t2;
	
	int pt1, pt2;
	
	pt1 = pthread_create(&t1, NULL, (void *)rd, (void *)&connfd);
	if(pt1 == -1)
	{
		printf("\nT1 fail!");
	}
	else
	{
		printf("\nT1 created!!");
	}
	
	pt2 = pthread_create(&t2, NULL, (void *)wt, (void *)&cfd);
	if(pt1 == -1)
	{
		printf("\nT2 fail!");
	}
	else
	{
		printf("\nT2 created!!");
	}
	
	printf("\n\t\t****************Server****************\n\n");
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	close(sockfd);
	close(cfd);
	
	
}
