#ifndef TUN_IF_H
#define TUN_IF_H

#include <linux/if.h>
#include <inttypes.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct dev {
    uint32_t ip_addrn;
    unsigned char mac[6];
} dev;

int set_ip(struct ifreq *ifr, int sock, char ip_addr[]);

int open_tun_dev(char *name, char *ip_addr, dev* device);

int get_mac_address(struct ifreq *ifr, int sock, unsigned char mac[6]);

#ifdef __cplusplus
}
#endif

#endif
