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
	acc.accept(sfd);
	cout<<"Connected!"<<endl;

	char buffer[50]; int sz;

	while(1)
	{
		sz = sfd.recv_n(buffer, 50);

		if(sz==0)
		{
			sfd.close();
			exit(0);
		}
		else
		{
			buffer[sz]='\0';
			printf("%s\n", buffer);
		}
	}
}
