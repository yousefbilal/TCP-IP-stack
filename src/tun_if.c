#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include "tun_if.h"


void open_tun_dev(char *name, char *ip_addr, tun_device* device) {
    struct ifreq ifr;
    int sock;

    if ((tun_fd = open("/dev/net/tun", O_RDWR)) < 0) {
        perror("Cannot open TUN/TAP device");
        exit(1);
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (name == NULL) {
        printf("device name has to be set\n");
        exit(1);
    }
    strncpy(ifr.ifr_name, name, IFNAMSIZ);

    if (ioctl(tun_fd, TUNSETIFF, (void *) &ifr) < 0) {
        perror("Could not ioctl tun");
        close(tun_fd);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Unable to create socket");
        close(tun_fd);
        exit(1);
    }
    if (set_ip(&ifr, sock, ip_addr) < 0) {
        close(tun_fd);
        exit(1);
    }
    device->ip_addrn = htonl(inet_network(ip_addr));

    ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
    if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
        perror("Unable to set state to UP");
        close(tun_fd);
        exit(1);
    }

    sleep(1);
    if (get_mac_address(&ifr, sock, device->mac) < 0) {
        close(tun_fd);
        exit(1);
    }
}

int set_ip(struct ifreq *ifr, int sock, char ip_addr[]) {
    struct sockaddr_in addr;
    int stat;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    stat = inet_pton(addr.sin_family, ip_addr, &addr.sin_addr);
    if (stat == 0)
        perror("Invalid IP");
    else if (stat == -1)
        perror("Invalid address family");

    if (stat != 1) {
        return -1;
    }
    ifr->ifr_addr = *(struct sockaddr *) &addr;

    if (ioctl(sock, SIOCSIFADDR, (void *) ifr) == -1) {
        perror("Unable to set ip address");
        return -1;
    }
    return 0;
}

int get_mac_address(struct ifreq *ifr, int sock, unsigned char mac[6]) {
    if (ioctl(sock, SIOCGIFHWADDR, ifr) < 0) {
        perror("Unable to get MAC address");
        return -1;
    }
    memcpy(mac, ifr->ifr_hwaddr.sa_data, 6);
    return 0;
}

void close_tun() {
    close(tun_fd);
}

ssize_t read_tun(unsigned char* buffer, size_t size) {
    return read(tun_fd, buffer, size);
}

void write_tun(unsigned char* buffer, size_t size) {
    write(tun_fd, buffer, size);
}