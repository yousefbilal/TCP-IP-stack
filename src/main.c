#include <stdio.h>
#include <string.h>
#include "tun_if.h"
#include "utils.h"
#include <stdlib.h>
#include <unistd.h>
#include "ether.h"
#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>
#include <linux/if_ether.h>
#include "defines.h"
#include "arp.h"

static bool keep_running = true;

void sig_handler(int _) {
    keep_running = false;
}

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGKILL, sig_handler);
    signal(SIGTERM, sig_handler);

    unsigned char buffer[MAX_PACKET_SIZE];

    dev device;

    int tun_fd = open_tun_dev(DEVICE_NAME, IP_ADDR, &device);
    if (tun_fd < 0) {
        exit(-1);
    }
    printf("MAC address of %s: ", DEVICE_NAME);
    print_mac(device.mac);

    translation_table *arp_table = malloc(sizeof(translation_table));
    init_translation_table(arp_table);

    while (keep_running) {
        ssize_t n = read(tun_fd, buffer, sizeof(buffer));
        if (n < 0)
            perror("Error reading from tun");

        eth_hdr *hdr = parse_eth_hdr(buffer);
        uint16_t ethertype = ntohs(hdr->ethertype);
        printf("Source MAC:");
        print_mac(hdr->smac);
        printf("Destination MAC:");
        print_mac(hdr->dmac);
        switch (ethertype) {
            case ETH_P_ARP:
                handle_arp(tun_fd, hdr, arp_table, &device);
                break;
            default:
                printf("Unknown ethertype\n");
        }
    }
    printf("Cleaning\n");
    close(tun_fd);
    return 0;
}
