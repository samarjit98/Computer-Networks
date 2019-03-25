#include "networks.h"

int main(int argc, char* argv[])
{
	struct sockaddr_in servip, peerip;
	int sfd, nsfd;

	if(( sfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
	{
		perror("Socket error!"); exit(0);
	}

	int reuse = 1;
	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
	{
		perror("Sockopt error!"); exit(0);
	}

	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servip.sin_port = htons(8000);

	// Tested for explicitly binded IP
	/*
	peerip.sin_family = AF_INET;
	peerip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	peerip.sin_port = htons(atoi(argv[1]));

	if(bind(sfd, (struct sockaddr*)&peerip, sizeof(peerip)) < 0)
	{
		perror("Bind error!"); exit(0);
	}
	*/

	if(connect(sfd, (struct sockaddr*)&servip, sizeof(servip)) < 0)
	{
		perror("Connect error!"); exit(0);
	}

	char buff[2];
	scanf("%s", buff);
	write(sfd, buff, 2);

	int peerlen = sizeof(peerip);
	getsockname(sfd, (struct sockaddr*)&peerip, &peerlen);

	close(sfd);

	if(( sfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
	{
		perror("Socket error!"); exit(0);
	}	

	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
	{
		perror("Sockopt error!"); exit(0);
	}

	if(bind(sfd, (struct sockaddr*)&peerip, sizeof(peerip)) < 0)
	{
		perror("Bind error!"); exit(0);
	}
	
	listen(sfd, LISTENQ);

	int flag=0;
	while(1)
	{
		nsfd = accept(sfd, NULL, NULL);

		if(nsfd>0)
		{
			char buff2[10];
			read(nsfd, buff2, 10);
			flag=1;
			printf("%s\n", buff2);
		}

		if(flag==1)break;
	}

	close(nsfd);
	close(sfd);

	return 0;
}

/*

Client output:

➜  Socket11_multiservice_unknown git:(master) ✗ ./client
1
Here
Service1
➜  Socket11_multiservice_unknown git:(master) ✗ ./client
1
Service1
➜  Socket11_multiservice_unknown git:(master) ✗ ./client
2
Service2
➜  Socket11_multiservice_unknown git:(master) ✗ ./client
1
Service1
➜  Socket11_multiservice_unknown git:(master) ✗ ./client
2
Service2

Server output:

➜  Socket11_multiservice_unknown git:(master) ✗ ./server
35244
1
35250
2
35256
1
35262
2

Service 1 output:

➜  Socket11_multiservice_unknown git:(master) ✗ ./service1
35244
35256

Service 2 output:

➜  Socket11_multiservice_unknown git:(master) ✗ ./service2
35250
35262

*/