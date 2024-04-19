#include "ether.h"


struct eth_hdr* parse_eth_hdr(unsigned char buff[]) {
    return (struct eth_hdr*) buff; 
}