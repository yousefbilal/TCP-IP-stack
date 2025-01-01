#include <stdint.h>
#include "ip.h"

// https://datatracker.ietf.org/doc/html/rfc1071
uint16_t checksum(void *addr, int count) {
   /* Compute Internet Checksum for "count" bytes
    *         beginning at location "addr".
    */

    register uint32_t sum = 0;
    uint16_t *addr_short = addr;
    while( count > 1 )  {
           sum += *addr_short++;
           count -= 2;
   }

    /*  Add left-over byte, if any */
   if( count > 0 )
        sum += *(uint8_t*) addr_short;

    /*  Fold 32-bit sum to 16 bits */
   while (sum>>16)
       sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}