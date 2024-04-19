#ifndef ETHER_H
#define ETHER_H

#include <linux/if_ether.h>
#include <stdint.h>

struct eth_hdr {
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed));

struct eth_hdr* parse_eth_hdr(unsigned char buff[]);

#endif