#ifndef IP_H
#define IP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


struct ip_hdr {
    uint8_t version : 4;
    uint8_t hdr_len : 4;
    uint8_t tos;
    uint16_t dgram_len;
    uint16_t id;
    uint16_t flags : 3;
    uint16_t frag_offset : 13;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
} __attribute__((packed));

typedef struct ip_hdr ip_hdr;

uint16_t checksum(void *addr, int count);

#ifdef __cplusplus
}
#endif


#endif //IP_H
