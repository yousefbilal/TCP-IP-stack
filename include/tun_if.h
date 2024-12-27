#ifndef TUN_IF_H
#define TUN_IF_H

#include <linux/if.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

static int tun_fd;

typedef struct tun_device {
    uint32_t ip_addrn;
    unsigned char mac[6];
} tun_device;

int set_ip(struct ifreq *ifr, int sock, char ip_addr[]);

void open_tun_dev(char *name, char *ip_addr, tun_device *device);

int get_mac_address(struct ifreq *ifr, int sock, unsigned char mac[6]);

void close_tun();

ssize_t read_tun(unsigned char* buffer, size_t size);

void write_tun(unsigned char* buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
