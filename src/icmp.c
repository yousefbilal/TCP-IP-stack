#include <netinet/ip_icmp.h>
#include <stdio.h>

#include "icmp.h"

void handle_icmp(ip_hdr *ip, tun_device *device) {
    icmp* icmp_packet = parse_icmp(ip->payload);

    switch (icmp_packet->type) {
        case ICMP_ECHO:

            break;
        default:
            printf("Unknown ICMP type\n");
    }

}

icmp *parse_icmp(uint8_t *data) {
    return (icmp *) data;
}

icmp_echo *parse_icmp_echo(uint8_t *data) {
    return (icmp_echo *) data;
}

icmp_dst_unreachable *parse_icmp_dst_unreachable(uint8_t *data) {
    return (icmp_dst_unreachable *) data;
}
