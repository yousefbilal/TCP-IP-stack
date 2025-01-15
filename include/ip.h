#ifndef IP_H
#define IP_H

#include <stdint.h>
#include "ether.h"
#include "tun_if.h"


#ifdef __cplusplus
extern "C" {
#endif

#define IP_HDR_SIZE 20

struct ip_hdr {
    uint8_t version: 4;
    uint8_t hdr_len: 4;
    uint8_t tos;
    uint16_t dgram_len;
    uint16_t id;
    uint16_t flags: 3;
    uint16_t frag_offset: 13;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
    uint8_t payload[];
} __attribute__((packed));

typedef struct ip_hdr ip_hdr;

uint16_t checksum(void *addr, int count);

ip_hdr *parse_ip(unsigned char *buffer);

void handle_ip(eth_hdr *eth, tun_device *device);

void send_ipv4(uint32_t saddr, uint32_t daddr, uint16_t id, uint16_t flags, uint16_t frag_offset,
               uint8_t proto, uint8_t *data, int data_size);

#ifdef __cplusplus
}
#endif


#endif //IP_H
