#include <stdint.h>
#include "ip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "icmp.h"

// https://datatracker.ietf.org/doc/html/rfc1071
uint16_t checksum(void *addr, int count) {
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     */

    register uint32_t sum = 0;
    uint16_t *addr_short = addr;
    while (count > 1) {
        sum += *addr_short++;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if (count > 0)
        sum += *(uint8_t *) addr_short;

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

ip_hdr *parse_ip(unsigned char *buffer) {
    return (ip_hdr *) buffer;
}

void handle_ip(eth_hdr *eth, tun_device *device) {
    ip_hdr *ip = parse_ip(eth->payload);
    uint16_t csum = checksum(ip, IP_HDR_SIZE);
    if (csum != 0) {
        printf("Erroneous ip header checksum\n");
        return;
    }
    switch (ip->proto) {
        case IPPROTO_ICMP:
            handle_icmp(ip, device);
            break;
        default:
            printf("Unknown IP encapsulated protocol\n");
    }
}

void send_ipv4(uint32_t saddr, uint32_t daddr, uint16_t id, uint16_t flags, uint16_t frag_offset,
               uint8_t proto, uint8_t *data, int data_size) {
    uint16_t total_size = IP_HDR_SIZE + data_size;
    ip_hdr *ip = (ip_hdr *)malloc(total_size);
    ip->version = 4;
    ip->hdr_len = 5;
    ip->tos = 0x00;
    //Reserved | Don't Fragment | More to come
    ip->flags = 0x2;
    ip->frag_offset = 0;

    static uint16_t ip_id = 0;
    ip->id = htons(ip_id++);
    ip->csum = 0;
    ip->ttl = 64;
    ip->proto = proto;
    ip->dgram_len = htons(total_size);
    ip->saddr = saddr;
    ip->daddr = daddr;
    ip->csum = checksum(ip, IP_HDR_SIZE);
    memcpy(ip->payload, data, total_size);


    free(ip);
}
