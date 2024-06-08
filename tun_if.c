#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>
#include "tun_if.h"
#include "utils.h"


int open_tun_dev(device* dev)
{
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
        print_error("Cannot open TUN/TAP dev");
        exit(1);
    }

    memset(&ifr, 0, sizeof(ifr));
    
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if( *dev->device ) {
        strncpy(ifr.ifr_name, dev->device, IFNAMSIZ);
    }
    

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        print_error("Could not ioctl tun");
        close(fd);
        return err;
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        print_error("Unable to create socket");
        close(fd);
        return -1;
    }
    if(setip(&ifr, sock, dev->ip_addr) < 0) {
        close(fd);
        return -1;
    }
    ifr.ifr_flags |=  IFF_UP | IFF_RUNNING;

    if(ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
        print_error("Unable to set state to UP");
        close(fd);
        return -1;
    }

    return fd;
}            

int setip (struct ifreq* ifr, int sock, char ip_addr[]) {

    struct sockaddr_in addr;
    int stat;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    // s = socket(addr.sin_family, SOCK_STREAM, 0);
    stat = inet_pton(addr.sin_family, ip_addr, &addr.sin_addr);
    if (stat == 0)
        printf("Invalid ip");
    else if (stat == -1)
        printf("Invalid address family");
    
    if (stat != 1) {
        return -1;
    
    }
    ifr->ifr_addr = *(struct sockaddr *) &addr;

    if (ioctl(sock, SIOCSIFADDR, (void*) ifr) == -1) {
        print_error("Unable to set ip address");
        return -1;
    }
    return 0;
}