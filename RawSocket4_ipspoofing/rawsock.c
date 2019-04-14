#include "networks.h"
#define PCKT_LEN 8192

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

int main(int argc, char *argv[]){
	int sd;
	char buffer[PCKT_LEN];
	struct ipheader *ip = (struct ipheader *) buffer;
	struct sockaddr_in sin, din;
	int one = 1;
	const int *val = &one;
	memset(buffer, 0, PCKT_LEN);
	sd = socket(PF_INET, SOCK_RAW, 20);
	if(sd < 0){
		perror("socket() error"); exit(-1);
	}
	else	printf("socket() - Using SOCK_RAW socket.\n");
	
	sin.sin_family = AF_INET;
	din.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	din.sin_addr.s_addr = inet_addr(argv[2]);

	ip->iph_ihl = 5;
	ip->iph_ver = 4;
	ip->iph_tos = 16; 
	ip->iph_len = sizeof(struct ipheader);
	ip->iph_ident = htons(54321);
	ip->iph_ttl = 64; // hops
	ip->iph_protocol = 20; 
	ip->iph_sourceip = inet_addr(argv[1]);
	ip->iph_destip = inet_addr(argv[3]);
	ip->iph_chksum = csum((unsigned short *)buffer, sizeof(struct ipheader));

	if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0){
		perror("setsockopt() error"); exit(-1);
	}
	else	printf("setsockopt() is OK.\n");

	printf("Trying...\n");
	printf("Using raw socket and UDP protocol\n");	
	printf("Using Source IP: %s, Target IP: %s.\n", argv[1], argv[2]);

	int count;
	for(count = 1; count <=20; count++){
		if(sendto(sd, buffer, ip->iph_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0){
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