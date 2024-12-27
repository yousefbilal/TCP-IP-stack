#ifndef ETHER_H
#define ETHER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


struct eth_hdr {
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed));

typedef struct eth_hdr eth_hdr;

eth_hdr *parse_eth_hdr(unsigned char buff[]);

void handle_eth_hdr(eth_hdr *hdr);

#ifdef __cplusplus
}
#endif

#endif
