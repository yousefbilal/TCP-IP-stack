#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


struct icmp_v4 {
    uint8_t type;
    uint8_t code;
    uint16_t csum; //with payload
    uint8_t data[];
} __attribute__((packed));

struct icmp_v4_echo {
    uint16_t id;
    uint16_t seq;
    uint8_t data[];
} __attribute__((packed));

struct icmp_v4_dst_unreachable {
    uint8_t unused;
    uint8_t len;
    uint16_t var;
    uint8_t data[];
} __attribute__((packed));

#ifdef __cplusplus
}
#endif

#endif //ICMP_H
