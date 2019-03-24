#include "networks.h"

int service_port[2] = {7000, 7001};

int main(int argc, char* argv[])
{
	struct sockaddr_in servip, peerip;
	int sfd, nsfd, peerlen;

	if(( sfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
	{
		perror("Socket error!"); exit(0);
	}

	servip.sin_family = AF_INET;
	servip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servip.sin_port = htons(8000);

	if(bind(sfd, (struct sockaddr*)&servip, sizeof(servip)) < 0)
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

				peerlen = sizeof(peerip);
				getpeername(nsfd, (struct sockaddr*)&peerip, &peerlen);

				int peerport = (int)ntohs(peerip.sin_port);

				printf("%d\n", peerport);

				char buff[2];
				read(nsfd, buff, 2);

				printf("%s\n", buff);

				if(buff[0]=='1')
				{
					struct sockaddr_in serviceip;
					int cfd;

					if(( cfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
					{
						perror("Socket error!"); exit(0);
					}	

					serviceip.sin_family = AF_INET;
					serviceip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
					serviceip.sin_port = htons(service_port[0]);

					if(connect(cfd, (struct sockaddr*)&serviceip, sizeof(serviceip)) < 0)
					{
						perror("Connect error!"); exit(0);
					}			

					write(cfd, &peerport, sizeof(int));

					close(cfd);
				}
				else if(buff[0]=='2')
				{
					struct sockaddr_in serviceip;
					int cfd;

					if(( cfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
					{
						perror("Socket error!"); exit(0);
					}	

					serviceip.sin_family = AF_INET;
					serviceip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
					serviceip.sin_port = htons(service_port[1]);

					if(connect(cfd, (struct sockaddr*)&serviceip, sizeof(serviceip)) < 0)
					{
						perror("Connect error!"); exit(0);
					}			

					write(cfd, &peerport, sizeof(int));

					close(cfd);
				}

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