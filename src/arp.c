#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/if_ether.h>

#include "ether.h"
#include "arp.h"


arp_hdr *parse_arp(unsigned char *buffer) {
    return (arp_hdr *) buffer;
}

arp_ipv4 *parse_arp_ipv4(arp_hdr *arp_hdr) {
    return (arp_ipv4 *) arp_hdr->data;
}

void set_entry(table_entry *node, uint32_t key, unsigned char value[6]) {
    node->key = key;
    memcpy(node->value, value, 6);
    node->next = NULL;
}

void init_translation_table() {
    arp_table.capacity = CAPACITY;
    arp_table.size = 0;
    arp_table.arr = (table_entry **) malloc(sizeof(table_entry *) * arp_table.capacity);
}

uint32_t hash_function(uint32_t key) {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key % arp_table.capacity;
}

void insert(uint32_t key, unsigned char value[6]) {
    uint32_t bucket_index = hash_function(key);
    table_entry *entry = malloc(sizeof(table_entry));
    set_entry(entry, key, value);
    if (arp_table.arr[bucket_index] == NULL) {
        arp_table.arr[bucket_index] = entry;
    } else {
        entry->next = arp_table.arr[bucket_index];
        arp_table.arr[bucket_index] = entry;
    }
}

void delete_entry(uint32_t key) {
    uint32_t bucket_index = hash_function(key);
    table_entry *temp = NULL;
    table_entry *current = arp_table.arr[bucket_index];

    while (current != NULL && current->key != key) {
        temp = current;
        current = current->next;
    }

    if (current == NULL)
        return;
    if (current == arp_table.arr[bucket_index])
        arp_table.arr[bucket_index] = current->next;
    else
        temp->next = current->next;
    free(current);
}

table_entry *search(uint32_t key) {
    uint32_t bucket_index = hash_function(key);
    table_entry *current = arp_table.arr[bucket_index];
    while (current != NULL && current->key != key) {
        current = current->next;
    }
    return current;
}

/*
 * ?Do I have the hardware type in ar$hrd?
 * Yes: (almost definitely)
 *   [optionally check the hardware length ar$hln]
 *   ?Do I speak the protocol in ar$pro?
 *   Yes:
 *     [optionally check the protocol length ar$pln]
 *     Merge_flag := false
 *     If the pair <protocol type, sender protocol address> is
 *         already in my translation table, update the sender
 *         hardware address field of the entry with the new
 *         information in the packet and set Merge_flag to true.
 *     ?Am I the target protocol address?
 *     Yes:
 *       If Merge_flag is false, add the triplet <protocol type,
 *           sender protocol address, sender hardware address> to
 *           the translation table.
 *       ?Is the opcode ares_op$REQUEST?  (NOW look at the opcode!!)
 *       Yes:
 *         Swap hardware and protocol fields, putting the local
 *             hardware and protocol addresses in the sender fields.
 *         Set the ar$op field to ares_op$REPLY
 *         Send the packet to the (new) target hardware address on
 *             the same hardware on which the request was received.
*/

void handle_arp(eth_hdr *eth, tun_device *device) {
    arp_hdr *arp = parse_arp(eth->payload);
    if (ntohs(arp->hwtype) != ARP_HTYPE_ETHER || ntohs(arp->protype) != ARP_PTYPE_IPV4)
        return;
    arp_ipv4 *arp_data = parse_arp_ipv4(arp);
    bool merge_flag = false;
    table_entry *entry = search(arp_data->sip);
    if (entry != NULL) {
        merge_flag = true;
        memcpy(entry->value, arp_data->smac, 6);
    }
    if (arp_data->dip != device->ip_addrn)
        return;

    if (!merge_flag)
        insert(arp_data->sip, arp_data->smac);

    if (ntohs(arp->opcode) != ARPOP_REQUEST)
        return;
    // TODO: clean this
    uint32_t temp_ip = arp_data->sip;
    unsigned char *temp_mac = arp_data->smac;
    arp_data->sip = device->ip_addrn;
    memcpy(arp_data->smac, device->mac, 6);
    arp_data->dip = temp_ip;
    memcpy(arp_data->dmac, temp_mac, 6);
    arp->opcode = htons(ARPOP_REPLY);
    memcpy(arp->data, (unsigned char *) arp_data, ARP_IPV4_SIZE);

    send_eth(device->mac, eth->smac, ETH_P_ARP, (unsigned char *) arp, ARP_HDR_SIZE + ARP_IPV4_SIZE);
}
