#include <stdio.h>
#include "utils.h"

void print_mac(unsigned char mac[6]) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
