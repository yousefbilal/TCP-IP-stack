#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>
#include <linux/if_ether.h>
#include "defines.h"
#include "arp.h"
#include "tun_if.h"
#include "utils.h"
#include "ether.h"
#include "ip.h"

static bool keep_running = true;

void sig_handler(int _) {
    keep_running = false;
}

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGKILL, sig_handler);
    signal(SIGTERM, sig_handler);

    unsigned char buffer[MAX_PACKET_SIZE];

    tun_device device;

    open_tun_dev(DEVICE_NAME, IP_ADDR, &device);

    printf("MAC address of %s: ", DEVICE_NAME);
    print_mac(device.mac);
    init_translation_table();
    uint16_t ethertype;
    eth_hdr *eth = NULL;
    while (keep_running) {
        if (read_tun(buffer, sizeof(buffer)) < 0)
            perror("Error reading from tun");

        eth = parse_eth_hdr(buffer);
        ethertype = ntohs(eth->ethertype);

        print_mac(eth->dmac);
        switch (ethertype) {
            case ETH_P_ARP:
                handle_arp(eth, &device);
                break;
            case ETH_P_IP:
                handle_ip(eth, &device);
                break;
            default:
                printf("Unknown ethertype\n");
        }
    }
    printf("Cleaning\n");
    close_tun();
    return 0;
}
