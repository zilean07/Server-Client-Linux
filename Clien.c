/* CLIENT SIDE */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define PORT2 8081
#define MAX 128

/* ******************FUNCTION STARTS****************** */

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
		else
		{
			printf("\nReading...");
		}
		
		printf("\nRecieved length : %d\n", *bufflen);
		
		int l;
		l = *bufflen;
		
		char *buff;
		buff = malloc(l);
		
		memset(buff, '\0', sizeof(buff));
		
		int rcv;
		
		rcv = read(*(int *)connfd, (void *)buff,l);
		if(rcv == -1)
		{
			printf("\nReceiving fail!!");
			free(bufflen);
			exit(0);
		}
		
		printf("\nServer --> %s", buff);
		
		if((strncmp("bye", buff, 3)) == -1)
		{
			flag = 0;
			exit(0);
		}
		
		memset(buff, '\0', sizeof(buff));
	}
}

void *wt(void *cfd)
{
	int flag = 1;
	
	while(flag)
	{
		int i = 0; 
		printf("\nClient : ");
		
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
		int *bufflen = &i;
		printf("Buffer lenght --> %d", *bufflen-1);
		
		int type;
		type = write(*(int *)cfd, buff, i);
		if(type == -1)
		{
			printf("Write : ");
			exit(0);
		}
		
		int wri1 = write(*(int *)cfd, buff, i);
		if(wri1 == -1)
		{
			perror("write : ");
			exit(0);
		}
		
		if((strncmp("bye", buff, 3)) == -1)
		{
			flag = 0;
			exit(0);
		}	
		
		memset(buff, '\0',  sizeof(buff));
	}
	
}

/* ******************FUNCTION ENDS****************** */


/* **************MAIN************** */
				
int main()
{
	/* *******Client******* */
	int cfd;
	struct sockaddr_in ca;
	
	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd == -1)
	{
		printf("\nMaster socket fail!!");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nSocket created");
	}
	//memset(ca, '\0', sizeof(ca));
	
	bzero(&ca, sizeof(ca));
	
	socklen_t len2;
	len2 = sizeof(ca);
	
	ca.sin_addr.s_addr = inet_addr("127.0.0.1");
	ca.sin_family = AF_INET;
	ca.sin_port = PORT;
	
	
	if((connect(cfd, (struct sockaddr*)&ca, len2)) == -1)
	{
		printf("\nConnection fail!\n");
		close(cfd);
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nConnected to server of A1");
	}
	
	/* *******Client******* */
	
	
	
	/* *******Server******* */
	int sockfd, connfd;
	struct sockaddr_in sa,cli;
	socklen_t len;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		printf("\nMaster socket fail!!");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\nSocket created");
	}
	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = PORT2;
	
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
	
//	struct sockaddr_in ca;
//	len = sizeof(cli);
	int len3;
	len3 = sizeof(cli);
	
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len2);
	
	if(connfd == -1)
	{
		perror("Accepting fail!");
		exit(0);
	}
	else
	{
		printf("\nAccepted");
	}
	
	/* *******Server******* */
	
	
	/* *********PTHREAD********* */
	
	pthread_t t1, t2;
	
	int pt1, pt2;
	
	pt1 = pthread_create(&t1, NULL, (void *)wt, (void *)&connfd);
	if(pt1 == -1)
	{
		printf("\nT1 fail!");
	}
	else
	{
		printf("\nT1 created!!");
	}
	
	pt2 = pthread_create(&t2, NULL, (void *)rd, (void *)&cfd);
	if(pt1 == -1)
	{
		printf("\nT2 fail!");
	}
	else
	{
		printf("\nT2 created!!");
	}
	
	printf("\n\t\t****************Client****************\n\n");
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	return 0;
}

/* **************MAIN ENDS************** */

