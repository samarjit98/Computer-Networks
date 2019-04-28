#include "networks.h"

int main(int argc, char* argv[]){
	int rawfd, udpfd;
	struct sockaddr_in rawip, udpip;
	struct sockaddr_in raddr;
	int rawiplen, udpiplen, raddrlen;

	char msg[2048]; int msglen;

	if(fork()==0){
		struct iphdr hdrip;
		struct udphdr hdrudp;

		int iphdrlen = sizeof(hdrip);
		int udphdrlen = sizeof(hdrudp);

		if((rawfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){  // IPPROTO_UDP
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
			hdrudp = *((struct udphdr*)(msg + iphdrlen));

			if(hdrudp.dest != rawip.sin_port)continue;

			printf("Raw socket:\n");
			printf("hl: %d, version: %d, ttl: %d, protocol: %d\n", hdrip.ihl, hdrip.version, hdrip.ttl, hdrip.protocol);
			printf("src: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.saddr)));
			printf("dest: %s\n", inet_ntoa(*((struct in_addr*)&hdrip.daddr)));
			printf("src port: %d, dest port: %d\n", ntohs(hdrudp.source), ntohs(hdrudp.dest));
			printf("UDP raw socket: payload: %s\n\n", msg+iphdrlen+udphdrlen);
		}
		close(rawfd);
	}
	else{
		if((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
			perror("Socket error!"); exit(0);
		}
		int reuse = 1;
		if(setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
			perror("Sockopt error!"); exit(0);
		}

		udpip.sin_family = AF_INET;
		udpip.sin_port = htons(atoi(argv[1]));
		udpip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		udpiplen = sizeof(udpip);

		if(bind(udpfd, (struct sockaddr*)&udpip, udpiplen) < 0){
			perror("Bind error!"); exit(0);
		}

		while(1){
			raddrlen = sizeof(raddr);
			msglen = recvfrom(udpfd, msg, 2048, 0, (struct sockaddr*)&raddr, &raddrlen);
			sleep(1);
			msg[2048] = 0;
			printf("UDP Socket:\n");
			printf("UDP: received length: %d, from: %s:%d\n", msglen, inet_ntoa(raddr.sin_addr), ntohs(raddr.sin_port));
			printf("UDP: payload: %s\n\n", msg);
		}
		close(udpfd);
	}
	return 0;
}

/*

➜  RawSocket_UDP git:(master) ✗ sudo ./raw 7000     
Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

Raw socket:
hl: 5, version: 4, ttl: 255, protocol: 17
src: 192.168.1.2
dest: 127.0.0.1
src port: 6666, dest port: 7000
UDP raw socket: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

UDP Socket:
UDP: received length: 26, from: 192.168.1.2:6666
UDP: payload: ABCDEFGHIJKLMNOPQRSTUVWXYZ>

*/