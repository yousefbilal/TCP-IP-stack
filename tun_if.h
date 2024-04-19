#ifndef TUN_IF_H
#define TUN_IF_H

#include <linux/if.h>

typedef struct {
    char device[IFNAMSIZ];
    char ip_addr[16]; 
} device;

int setip (struct ifreq* ifr, int sock, char ip_addr[]);
int open_tun_dev(device* dev);

#endif
