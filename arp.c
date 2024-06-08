#include "arp.h"

struct arp_ipv4* parse_arp_ipv4(unsigned char* buffer) {
    struct arp_hdr* hdr = (struct arp_hdr*) buffer;
    struct arp_ipv4* arp = (struct arp_ipv4*) hdr->data;
    return arp;
}
