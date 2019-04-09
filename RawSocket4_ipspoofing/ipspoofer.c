#include "networks.h"
#define PCKT_LEN 8192
/*
struct ipheader {
	unsigned char      iph_ihl:5, iph_ver:4;
	unsigned char      iph_tos;
	unsigned short int iph_len;
	unsigned short int iph_ident;
	unsigned char      iph_flag;
	unsigned short int iph_offset;
	unsigned char      iph_ttl;
	unsigned char      iph_protocol;
	unsigned short int iph_chksum;
	unsigned int       iph_sourceip;
	unsigned int       iph_destip;
};

unsigned short csum(unsigned short *buf, int nwords){    
	unsigned long sum;
	for(sum=0; nwords>0; nwords--)sum += *buf++;
	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}
*/
int main(int argc, char *argv[]){
	int sd;
	char buffer[PCKT_LEN];
	struct iphdr *ip = (struct iphdr*) buffer;
	struct sockaddr_in sin, din;
	int one = 1;
	const int *val = &one;
	memset(buffer, 0, PCKT_LEN);
	sd = socket(AF_INET, SOCK_RAW, 10);
	if(sd < 0){
		perror("socket() error"); exit(-1);
	}
	else	printf("socket() - Using SOCK_RAW socket.\n");
	
	sin.sin_family = AF_INET;
	din.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	din.sin_addr.s_addr = inet_addr(argv[2]);

	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 16; 
	ip->tot_len = sizeof(struct iphdr);
	ip->id = htons(54321);
	ip->ttl = 64; 
	ip->protocol = 10; 
	ip->saddr = inet_addr(argv[1]);
	ip->daddr = inet_addr(argv[2]);
	//ip->iph_chksum = csum((unsigned short *)buffer, sizeof(struct ipheader));

	if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0){
		perror("setsockopt() error"); exit(-1);
	}
	else	printf("setsockopt() is OK.\n");

	printf("Trying...\n");
	printf("Using raw socket\n");	
	printf("Using Source IP: %s, Target IP: %s.\n", argv[1], argv[2]);

	int count;
	for(count = 1; count <=20; count++){
		if(sendto(sd, buffer, ip->tot_len, 0, (struct sockaddr *)&din, sizeof(din)) < 0){
			perror("sendto() error"); exit(-1);
		}
		else{
			printf("Count #%u - sendto() is OK.\n", count);
			sleep(2);
		}
	}
	close(sd);
	return 0;
}

/*
Output:

➜  RawSocket4_ipspoofing git:(master) ✗ sudo ./ipspoofer 69.69.69.69 127.0.0.1
socket() - Using SOCK_RAW socket.
setsockopt() is OK.
Trying...
Using raw socket
Using Source IP: 69.69.69.69, Target IP: 127.0.0.1.
Count #1 - sendto() is OK.
Count #2 - sendto() is OK.
Count #3 - sendto() is OK.
Count #4 - sendto() is OK.
Count #5 - sendto() is OK.
Count #6 - sendto() is OK.
Count #7 - sendto() is OK.
Count #8 - sendto() is OK.
Count #9 - sendto() is OK.
Count #10 - sendto() is OK.
Count #11 - sendto() is OK.
Count #12 - sendto() is OK.
Count #13 - sendto() is OK.
Count #14 - sendto() is OK.
Count #15 - sendto() is OK.

*/