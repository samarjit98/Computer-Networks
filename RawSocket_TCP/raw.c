#include "networks.h"

int main(int argc, char* argv[]){
	int rawfd, tcpfd;
	struct sockaddr_in rawip, tcpip;
	struct sockaddr_in raddr;
	int rawiplen, tcpiplen, raddrlen;

	char msg[2048]; int msglen;

	if(fork()==0){
		struct iphdr hdrip;
		struct tcphdr hdrtcp;

		int iphdrlen = sizeof(hdrip);
		int tcphdrlen = sizeof(hdrtcp);

		if((rawfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0){  
			perror("Raw socket error!"); exit(0);
		}
		int reuse = 1;
		if(setsockopt(rawfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
			perror("Raw sockopt error!"); exit(0);
		}

		rawip.sin_family = AF_INET;
		rawip.sin_port = htons(atoi(argv[1]));
		rawip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		rawiplen = sizeof(rawip);

		if(bind(rawfd, (struct sockaddr*)&rawip, rawiplen) < 0){
			perror("Raw bind error!"); exit(0);
		}

		while(1){
			raddrlen = sizeof(raddr);
			msglen = recvfrom(rawfd, msg, 2048, 0, (struct sockaddr*)&raddr, &raddrlen);

			if(msglen <= 0)continue;

			hdrip = *((struct iphdr*)msg);
			hdrtcp = *((struct tcphdr*)(msg + iphdrlen));

			if(hdrtcp.dest != rawip.sin_port)continue;

			printf("Raw socket:\n");
			printf("hl: %d, version: %d, ttl: %d, protocol: %d\n", hdrip.ihl, hdrip.version, hdrip.ttl, hdrip.protocol);
			printf("src: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.saddr)));
			printf("dest: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.daddr)));
			printf("src port: %d, dest port: %d\n", ntohs(hdrtcp.source), ntohs(hdrtcp.dest));
			printf("TCP raw socket: payload: %s\n\n", msg+iphdrlen+tcphdrlen);
		}
		close(rawfd);
	}
	else{
		if((tcpfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			perror("Socket error!"); exit(0);
		}
		int reuse = 1;
		if(setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
			perror("Sockopt error!"); exit(0);
		}

		tcpip.sin_family = AF_INET;
		tcpip.sin_port = htons(atoi(argv[1]));
		tcpip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		tcpiplen = sizeof(tcpip);

		if(bind(tcpfd, (struct sockaddr*)&tcpip, tcpiplen) < 0){
			perror("Bind error!"); exit(0);
		}
		//listen(tcpfd, LISTENQ);

		/*
		while(1){
			raddrlen = sizeof(raddr);
			int nsfd = accept(tcpfd, (struct sockaddr*)&raddr, &raddrlen);

			if(nsfd>0){
				if(fork()==0){
					while((msglen = recv(nsfd, msg, 2048, 0))>0){
						sleep(1);
						msg[2048] = 0;
						printf("TCP Socket:\n");
						printf("TCP: received length: %d, from: %s:%d\n", msglen, inet_ntoa(raddr.sin_addr), ntohs(raddr.sin_port));
						printf("TCP: payload: %s\n\n", msg);
					}
				}
				else{
					close(nsfd);
				}
			}
		}
		*/
		while(1){
			raddrlen = sizeof(raddr);
			msglen = recvfrom(tcpfd, msg, 2048, 0, (struct sockaddr*)&raddr, &raddrlen);
			if(msglen==-1)continue;
			sleep(1);
			msg[2048] = 0;
			printf("TCP Socket:\n");
			printf("TCP: received length: %d, from: %s:%d\n", msglen, inet_ntoa(raddr.sin_addr), ntohs(raddr.sin_port));
			printf("TCP: payload: %s\n\n", msg);
		}
		close(tcpfd);
	}
	return 0;
}

/*

Output:

Raw socket:
hl: 5, version: 4, ttl: 64, protocol: 6
src: 127.0.0.1
dest: 127.0.0.1
src port: 12878, dest port: 5000
TCP raw socket: payload: ��
A�ڸ

Raw socket:
hl: 5, version: 4, ttl: 64, protocol: 6
src: 127.0.0.1
dest: 127.0.0.1
src port: 12878, dest port: 5000
TCP raw socket: payload: 
A�ڸA�ڸA�ڸ@

Raw socket:
hl: 5, version: 4, ttl: 64, protocol: 6
src: 127.0.0.1
dest: 127.0.0.1
src port: 12878, dest port: 5000
TCP raw socket: payload: 
A���A�ڸHello

TCP Socket:
TCP: received length: 6, from: 127.0.0.1:12878
TCP: payload: Hello

*/