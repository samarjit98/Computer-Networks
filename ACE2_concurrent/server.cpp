#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

int main()
{
	ACE_INET_Addr addr(5001, INADDR_LOOPBACK);
	ACE_SOCK_Stream sfd;

	ACE_SOCK_Acceptor acc(addr);

	char buffer[50]; int sz;

	while(1)
	{
		acc.accept(sfd);
		cout<<"Connected!"<<endl;

		if(fork()==0)
		{
			while(1)
			{
				sz = sfd.recv_n(buffer, 50);

				if(sz==0)break;

				buffer[sz]='\0';
				printf("%d: %s\n", (int)getpid(), buffer);

				scanf("%s", buffer);
				sfd.send_n(buffer, 50);
			}

			sfd.close();
		}
		else
		{
			sfd.close();
		}
	}
	return 0;
}
