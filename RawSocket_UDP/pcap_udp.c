/*
	Raw UDP sockets
*/
/*
#include <stdio.h>	//for printf
#include <string.h> //memset
#include <sys/socket.h>	//for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/udp.h>	//Provides declarations for udp header
#include <netinet/ip.h>	//Provides declarations for ip header
*/
#include "networks.h"
#include <pcap.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
/* 
	96 bit (12 bytes) pseudo header needed for udp header checksum calculation 
*/
struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

/*
	Generic checksum calculation function
*/
unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

int main (int argc, char* argv[])
{
	//Create a raw socket of type IPPROTO
	int s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
	
	if(s == -1)
	{
		//socket creation failed, may be because of non-root privileges
		perror("Failed to create raw socket");
		exit(1);
	}
	
	//Datagram to represent the packet
	char datagram[4096] , source_ip[32] , *data , *pseudogram;
	
	//zero out the packet buffer
	memset (datagram, 0, 4096);
	
	//IP header
	struct ether_header *header = (struct ether_header*)datagram;
	struct ether_header tmp_hdr;
    tmp_hdr.ether_type=htons(ETHERTYPE_IP);
    //memset(tmp_hdr.ether_dhost, 0xFF, sizeof(tmp_hdr.ether_dhost));
    tmp_hdr.ether_dhost[0] = 0xec;
    tmp_hdr.ether_dhost[1] = 0x8e;
    tmp_hdr.ether_dhost[2] = 0xb5;
    tmp_hdr.ether_dhost[3] = 0x54;
    tmp_hdr.ether_dhost[4] = 0xda;
    tmp_hdr.ether_dhost[5] = 0xdb;
    tmp_hdr.ether_shost[0] = 0xec;
    tmp_hdr.ether_shost[1] = 0x8e;
    tmp_hdr.ether_shost[2] = 0xb5;
    tmp_hdr.ether_shost[3] = 0x54;
    tmp_hdr.ether_shost[4] = 0xda;
    tmp_hdr.ether_shost[5] = 0xdb;
    memcpy(header, &tmp_hdr, sizeof(struct ether_header));

	struct iphdr *iph = (struct iphdr *) (datagram + sizeof(struct ether_header));
	
	//UDP header
	struct udphdr *udph = (struct udphdr *) (datagram + sizeof(struct ether_header) + sizeof (struct iphdr));
	
	struct sockaddr_in sin;
	struct pseudo_header psh;
	
	//Data part
	data = datagram + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr);
	strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	//some address resolution
	strcpy(source_ip , "192.168.1.2");
	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[1]));
	sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	//Fill in the IP Header
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
	iph->id = htonl (54321);	//Id of this packet
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_UDP;
	iph->check = 0;		//Set to 0 before calculating checksum
	iph->saddr = inet_addr ( source_ip );	//Spoof the source ip address
	iph->daddr = sin.sin_addr.s_addr;
	
	//Ip checksum
	//iph->check = csum ((unsigned short *) datagram, iph->tot_len);
	
	//UDP header
	udph->source = htons (6666);
	udph->dest = htons (atoi(argv[1]));
	udph->len = htons(8 + strlen(data));	//tcp header size
	udph->check = 0;	//leave checksum 0 now, filled later by pseudo header
	
	//Now the UDP checksum using the pseudo header
	
	psh.source_address = inet_addr(source_ip);
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );
	
	int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
	pseudogram = malloc(psize);
	
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
	
	udph->check = csum( (unsigned short*) pseudogram , psize);
	
	//loop if you want to flood :)
	char pcap_errbuf[PCAP_ERRBUF_SIZE];
    pcap_errbuf[0]='\0';
    pcap_t* pcap = pcap_open_live("eno1", 96, 0, 0, pcap_errbuf);
    if (pcap_errbuf[0]!='\0') {
        fprintf(stderr,"%s\n",pcap_errbuf);
    }
    if (!pcap) {
        exit(1);
    }

    // Write the Ethernet frame to the interface.
    for(int i=0; i<10; i++){
	    if (pcap_sendpacket(pcap, datagram, sizeof(struct ether_header) + sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data))==-1) {
	        pcap_perror(pcap, 0);
	        pcap_close(pcap);
	        exit(1);
	    }
	    else printf("Sent\n");
	}

    // Close the PCAP descriptor.
    pcap_close(pcap);
    return 0;
}
