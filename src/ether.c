#include "ether.h"

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "tun_if.h"
#include "utils.h"

//TODO: Properly serialize/deserialize the struct
eth_hdr* parse_eth_hdr(unsigned char buff[]) {
    return (eth_hdr*) buff;
}

void send_eth(unsigned char *smac, unsigned char *dmac, uint16_t ethertype, unsigned char *data, int data_size) {
    int total_size = ETHER_SIZE + data_size;
    eth_hdr *eth = malloc(total_size);
    memcpy(eth->dmac, dmac, 6);
    memcpy(eth->smac, smac, 6);
    memcpy(eth->payload, data, data_size);
    eth->ethertype = htons(ethertype);
    write_tun((unsigned char*) eth, total_size);
    print_eth_frame(eth, data_size);
    free(eth);
}

void print_eth_frame(eth_hdr* eth, int payload_size) {
    printf("Ethernet Frame:\n");

    printf("\tDestination MAC: ");
    print_mac(eth->dmac);
    printf("\tSource MAC: ");
    print_mac(eth->smac);
    printf("\tEtherType: 0x%04x\n", ntohs(eth->ethertype));

    printf("\tPayload (%zu bytes):", payload_size);
    for (int i = 0; i < payload_size; i++) {
        if (i % 16 == 0) printf("\n\t");
        printf("%02x ", eth->payload[i]);
    }
    printf("\n");
}