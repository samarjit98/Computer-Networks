#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

int main()
{
	ACE_INET_Addr addr(5002, INADDR_LOOPBACK);
	ACE_SOCK_Stream sfd;
	ACE_SOCK_Connector con;

	con.connect(sfd, addr);
	cout<<"Connected!"<<endl;

	char buffer[50]; int sz;

	while(1)
	{
		scanf("%s", buffer);
		sfd.send_n(buffer, 50);

		sz = sfd.recv_n(buffer, 50);
		buffer[sz]='\0';
		printf("%s\n", buffer);
	}
	return 0;
}
