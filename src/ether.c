#include "ether.h"

//TODO: Properly serialize/deserialize the struct
eth_hdr* parse_eth_hdr(unsigned char buff[]) {
    return (eth_hdr*) buff;
}