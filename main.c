#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "tun_if.h"
#include "utils.h"
#include <stdlib.h>
#include <unistd.h>
#include "ether.h"
#include <arpa/inet.h>
#include "arp.h"


#define MAX_PACKET_SIZE 2048

// write a function to display the mac address from an unsigned char[6]
void print_mac(unsigned char mac[6]) {
    for(int i = 0; i < 6; i++) {
        printf("%02x", mac[i]);
        if(i < 5) {
            printf(":");
        }
    }
}


typedef struct {
    unsigned char data[MAX_PACKET_SIZE];
    size_t length;
} packet;

int main() {
    unsigned char buffer[2048];
    char tun_name[IFNAMSIZ] = "tap0";

    device dev;
    
    strcpy(dev.ip_addr, "10.0.0.1");
    strcpy(dev.device, tun_name);
    int tun_fd = open_tun_dev(&dev);
    if(tun_fd < 0) {
        exit(-1);
    }
    while(1) {
        packet pkt;
        ssize_t n = read(tun_fd, buffer, sizeof(buffer));
        if(n < 0) 
            print_error("Error reading from tun");

        memcpy(pkt.data, buffer, n);
        pkt.length = n;
        struct eth_hdr* hdr = parse_eth_hdr(buffer);

        if(ntohs(hdr->ethertype) == ETH_P_ARP) {
            printf("Source MAC:");
            print_mac(hdr->smac);
            printf("\n");
            printf("Destination MAC:");
            print_mac(hdr->dmac);
            printf("\n");
            printf("payload: %p\n", hdr->payload);
            printf("\n\n");
            struct arp_ipv4* arp = parse_arp_ipv4(hdr->payload);

            struct in_addr saddr;
            saddr.s_addr = arp->sip;
            struct in_addr daddr;
            daddr.s_addr = arp->dip;
            printf("sip: %s | dip: %s \n", inet_ntoa(saddr), inet_ntoa(daddr));
        }


    }

    return 0;
}