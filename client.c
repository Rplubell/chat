#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLENG 100

struct ThreadData
{
	int sfd;
	int dfd;
};

void* reader(void * td)
{
	struct ThreadData* data = (struct ThreadData*) td;
	int sfd = data->sfd;
	int dfd = data->dfd;
	long n;
	char str[MAXLENG];
	for(;;)
	{
		bzero(str, MAXLENG);
		n = read(sfd, str, MAXLENG);
		if(n < 0) return NULL;
		n = write(dfd, str, sizeof(str)+1);
		if(n < 0) return NULL;
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));

	inet_pton(AF_INET,"173.236.194.161",&(servaddr.sin_addr));

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	pthread_t thread[1];
	struct ThreadData data[1];

	data[0].sfd = 0;
	data[0].dfd = sockfd;
	data[1].sfd = sockfd;
	data[1].dfd = 1;

	int j;
	for(j = 0; j <= 1; j++)
		pthread_create(&thread[j], NULL, *reader, &data[j]);

	for(j = 0; j <= 1; j++)
		pthread_join(thread[j],NULL);

	/*while(1)
	{
		bzero(sendline, 100);
		bzero(recvline, 100);
		fgets(sendline, 100, stdin);

		write(sockfd, sendline, strlen(sendline)+1);
		read(sockfd, recvline, 100);
		printf("%s\n", recvline);
	}*/
	return 0;
}
