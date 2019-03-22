#include <bits/stdc++.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/Handle_Set.h>
#include <bits/stdc++.h>
#include <stdio.h>

static ACE_SOCK_Acceptor acceptor1, acceptor2;
static ACE_Handle_Set activity_handles;
static ACE_Handle_Set ready_handles;
static void initialize_acceptor()
{
	ACE_INET_Addr saddr1(5001, INADDR_LOOPBACK);
	acceptor1.open(saddr1);
	//acceptor1.enable(ACE_NONBLOCK);
	activity_handles.set_bit(acceptor1.get_handle());

	ACE_INET_Addr saddr2(5002, INADDR_LOOPBACK);
	acceptor2.open(saddr2);
	//acceptor2.enable(ACE_NONBLOCK);
	activity_handles.set_bit(acceptor2.get_handle());
}

static void handle_connections (void) 
{
	if (ready_handles.is_set(acceptor1.get_handle ()))
	{
		ACE_SOCK_Stream str;
		acceptor1.accept(str);

		printf("Acceptor 1 connected!!\n");
		
		char buffer[50]; int sz;
		sz = str.recv_n(buffer, 50);

        buffer[sz]='\0';
        printf("%d: Acceptor 1 %s\n", (int)getpid(), buffer);

        scanf("%s", buffer);
        str.send_n(buffer, 50);

		str.close();
	}
	else if (ready_handles.is_set(acceptor2.get_handle ()))
	{
		ACE_SOCK_Stream str;
		acceptor2.accept(str);

		printf("Acceptor 2 connected!!\n");
		
		char buffer[50]; int sz;
		sz = str.recv_n(buffer, 50);

        buffer[sz]='\0';
        printf("%d: Acceptor 2 %s\n", (int)getpid(), buffer);

        scanf("%s", buffer);
        str.send_n(buffer, 50);

		str.close();
	}
}

int main(int argc, char* argv[])
{
	initialize_acceptor();
	
	for (;;) 
	{
		ready_handles.reset();
		ready_handles = activity_handles;
		ACE::select(ACE_DEFAULT_SELECT_REACTOR_SIZE, ready_handles);
		handle_connections();
	}
}
