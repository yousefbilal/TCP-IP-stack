#ifndef ETHER_H
#define ETHER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ETHER_SIZE 14

struct eth_hdr {
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed));

typedef struct eth_hdr eth_hdr;

eth_hdr *parse_eth_hdr(unsigned char buff[]);

void handle_eth_hdr(eth_hdr *hdr);

void send_eth(unsigned char *smac, unsigned char *dmac, uint16_t ethertype, unsigned char *data, int data_size);

void print_eth_frame(eth_hdr* eth, int payload_size);

#ifdef __cplusplus
}
#endif

#endif //ETHER_H
