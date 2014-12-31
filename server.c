#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLENG 100
#define PORT 100

struct ThreadData
{
	int sfd;
	int dfd;
};

struct AcceptorData
{
	int lfd;
	int amtThreads;
};

void* reader(void * td)
{
	printf("Readddd\n");
	struct ThreadData* data = (struct ThreadData*) td;
	int sfd = data->sfd;
	int dfd = data->dfd;
	char str[MAXLENG];
	for(;;)
	{
		bzero(str, MAXLENG);
		if(read(sfd,str,MAXLENG) < 0) return NULL;
		printf("%s", str);
		if(write(dfd, str, strlen(str)+1) < 0) return NULL;
	}

	return NULL;
}

void* acceptor(void * AcceptorData)
{
	int fd = accept(listen_fd, (struct sockaddr *) NULL NULL);
	printf("
}

int main(int argc, char **argv)
{
	pthread_t thread[1];
	struct ThreadData data[1];

	int listen_fd, client0_fd, client1_fd;

	struct sockaddr_in servaddr;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listen_fd, 10);

	client0_fd = accept(listen_fd, (struct sockaddr *) NULL, NULL);
	printf("1 connected\n");
	client1_fd = accept(listen_fd, (struct sockaddr *) NULL, NULL);
	printf("2 connected\n");

	data[0].sfd = client0_fd;
	data[0].dfd = client1_fd;
	data[1].sfd = client1_fd;
	data[1].dfd = client0_fd;

	int j;
	for(j = 0; j <= 1; j++)
		pthread_create(&thread[j], NULL, *reader, &data[j]);

	for(j = 0; j <= 1; j++)
		pthread_join(thread[j], NULL);
	/*while(1)
	{
		getit(client1_fd, client2_fd);
		bzero(str, MAXLENG);

		read(client1_fd,str,MAXLENG);

		printf("%s", str);

		write(client2_fd, str, strlen(str)+1);
	}*/
	return 0;
}
