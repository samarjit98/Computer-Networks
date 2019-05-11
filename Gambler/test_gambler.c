#include "networks.h"

typedef struct{
	char ip_addr[20];
	u_char mac_addr[ETHER_ADDR_LEN];
} table_t;

table_t lookup[1000]; int curr_ptr = 0;
FILE *logfile;
void process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
void process_ip_packet(const u_char * , int);
void print_ip_packet(const u_char * , int);
void print_tcp_packet(const u_char *  , int );
void print_udp_packet(const u_char * , int);
void print_icmp_packet(const u_char * , int );
void PrintData (const u_char * , int);
void gambler(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer);
struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j; 

char from[20], to[20];
pcap_t *handle; 
 
int main()
{
    pcap_if_t *alldevsp , *device; //Handle of the device that shall be sniffed
 
    char errbuf[100] , *devname , devs[100][100];
    int count = 1 , n;
     
    // get the list of available devices
    /*
    printf("Finding available devices ... ");
    if( pcap_findalldevs( &alldevsp , errbuf) )
    {
        printf("Error finding devices : %s" , errbuf);
        exit(1);
    }
    printf("Done");
     
    //Print the available devices
    printf("\nAvailable Devices are :\n");
    for(device = alldevsp ; device != NULL ; device = device->next)
    {
        printf("%d. %s - %s\n" , count , device->name , device->description);
        if(device->name != NULL)
        {
            strcpy(devs[count] , device->name);
        }
        count++;
    }
     
    //Ask user which device to sniff
    printf("Enter the number of the device you want to sniff : ");
    scanf("%d" , &n);
    devname = devs[n];
    //Open the device for sniffing
    printf("Opening device %s for sniffing ... " , devname);
    */
    handle = pcap_open_live("eno1" , 65535 , 1 , 0 , errbuf);
     
    if (handle == NULL) 
    {
        fprintf(stderr, "Couldn't open device %s : %s\n" , devname , errbuf);
        exit(1);
    }

    logfile=fopen("log.txt","w");
    if(logfile==NULL) 
    {
        printf("Unable to create file.");
    }

    pcap_loop(handle , 10 , process_packet , NULL); 

    //printf("Enter ip you want to re-route:\n"); scanf("%s", from); scanf("%s", to);

    fprintf(logfile, "\n\n***************************************\n\nREROUTING FROM NOW\n\n");
    for(int i=0; i<curr_ptr; i++){
    	printf("IP Addr: %s, MAC addr: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", lookup[i].ip_addr, lookup[i].mac_addr[0], lookup[i].mac_addr[1], lookup[i].mac_addr[2], lookup[3].mac_addr[4], lookup[i].mac_addr[5], lookup[i].mac_addr[6]);
    } 

    pcap_loop(handle, 0, gambler, NULL);

    return 0;   
}

void print_ethernet_header(const u_char *Buffer, int Size)
{
    struct ethhdr *eth = (struct ethhdr *)Buffer;
     
    fprintf(logfile , "\n");
    fprintf(logfile , "Ethernet Header\n");
    fprintf(logfile , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    fprintf(logfile , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    fprintf(logfile , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}
 
void print_ip_header(const u_char * Buffer, int Size)
{
    //print_ethernet_header(Buffer , Size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    fprintf(logfile , "   |-IP Version        : %d\n",(unsigned int)iph->version);
    fprintf(logfile , "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    fprintf(logfile , "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    fprintf(logfile , "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    fprintf(logfile , "   |-Identification    : %d\n",ntohs(iph->id));
    fprintf(logfile , "   |-TTL      : %d\n",(unsigned int)iph->ttl);
    fprintf(logfile , "   |-Protocol : %d\n",(unsigned int)iph->protocol);
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(iph->check));
    fprintf(logfile , "   |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
    fprintf(logfile , "   |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
}

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
    int size = header->len;

    print_ethernet_header(buffer, size);
    print_ip_header(buffer, size);
    struct ethhdr *eth = (struct ethhdr*)(buffer);    
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    
   	table_t src, dest;
   	struct sockaddr_in srcip, destip;
   	src.mac_addr[0] = eth->h_source[0];
   	src.mac_addr[1] = eth->h_source[1];
   	src.mac_addr[2] = eth->h_source[2];
   	src.mac_addr[3] = eth->h_source[3];
   	src.mac_addr[4] = eth->h_source[4];
   	src.mac_addr[5] = eth->h_source[5];
   	dest.mac_addr[0] = eth->h_dest[0];
   	dest.mac_addr[1] = eth->h_dest[1];
   	dest.mac_addr[2] = eth->h_dest[2];
   	dest.mac_addr[3] = eth->h_dest[3];
   	dest.mac_addr[4] = eth->h_dest[4];
   	dest.mac_addr[5] = eth->h_dest[5];
   	srcip.sin_addr.s_addr = iph->saddr;
   	destip.sin_addr.s_addr = iph->daddr;
   	strcpy(src.ip_addr, inet_ntoa(srcip.sin_addr));
   	strcpy(dest.ip_addr, inet_ntoa(destip.sin_addr));
   	int flag1=0, flag2=0;
   	for(int i=0; i<curr_ptr; i++){
   		if(strcmp(lookup[i].ip_addr, src.ip_addr)==0)flag1=1;
   		if(strcmp(lookup[i].ip_addr, dest.ip_addr)==0)flag2=1;
   	}
   	if(flag1==0){lookup[curr_ptr] = src; curr_ptr++;}
   	if(flag2==0){lookup[curr_ptr] = dest; curr_ptr++;}
}
 
void gambler(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
	int size = header->len;
	struct iphdr *iph_tmp = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    u_char cpy_buffer[4096];
    //printf("Here\n");
    memcpy(cpy_buffer, buffer, size);

    struct sockaddr_in destip;
    destip.sin_addr.s_addr = iph_tmp->daddr;
    char destips[20];
    strcpy(destips, inet_ntoa(destip.sin_addr));
    if(strcmp(destips, "192.168.105.240")==0)
    {
        table_t refer;
        refer = lookup[(int)(rand() % curr_ptr)];
        	

        struct ethhdr *eth = (struct ethhdr*)(cpy_buffer);    
        struct iphdr *iph = (struct iphdr*)(cpy_buffer + sizeof(struct ethhdr));

        eth->h_dest[0] = 0xff;
        eth->h_dest[1] = 0xff;
        eth->h_dest[2] = 0xff;
        eth->h_dest[3] = 0xff;
        eth->h_dest[4] = 0xff;
        eth->h_dest[5] = 0xff;
        iph->daddr = inet_addr("192.168.105.2");
        fprintf(logfile, "\nLets Gamble now:\n");
        fprintf(logfile, "\nORIGINAL:\n");
        print_ethernet_header(buffer, size);
        print_ip_header(buffer, size);
        fprintf(logfile, "\nGAMBLED:\n");
        print_ethernet_header(cpy_buffer, size);
        print_ip_header(cpy_buffer, size);

        pcap_inject(handle, cpy_buffer, size);
    }
}
