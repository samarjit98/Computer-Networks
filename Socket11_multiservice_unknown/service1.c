#include "networks.h"

int main(int argc, char* argv[])
{
	struct sockaddr_in serviceip;
	int sfd, nsfd;

	if(( sfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
	{
		perror("Socket error!"); exit(0);
	}

	serviceip.sin_family = AF_INET;
	serviceip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serviceip.sin_port = htons(7000);

	if(bind(sfd, (struct sockaddr*)&serviceip, sizeof(serviceip)) < 0)
	{
		perror("Bind error!"); exit(0);
	}

	listen(sfd, LISTENQ);

	while(1)
	{
		nsfd = accept(sfd, NULL, NULL);

		if(nsfd>0)
		{
			if(fork()==0)
			{
				close(sfd);

				struct sockaddr_in peerip;
				int peerport;

				read(nsfd, &peerport, sizeof(int));
				printf("%d\n", peerport);

				peerip.sin_family = AF_INET;
				peerip.sin_addr.s_addr = INADDR_LOOPBACK;
				peerip.sin_port = htons(peerport);

				int cfd;
				if(( cfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
				{
					perror("Socket error!"); exit(0);
				}

				if(connect(cfd, (struct sockaddr*)&peerip, sizeof(peerip)) < 0)
				{
					perror("Connect error!"); exit(0);
				}

				char buff[10] = "Service1";

				printf("Here\n");
				write(cfd, buff, strlen(buff)+1);

				close(cfd);
				close(nsfd);
			}
			else
			{
				close(nsfd);
			}
		}
	}
	return 0;
}