#ifndef ARP_H
#define ARP_H

#include "ether.h"
#include "tun_if.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ARP protocol opcodes. */
#define	ARPOP_REQUEST	1		/* ARP request			*/
#define	ARPOP_REPLY	    2		/* ARP reply			*/
#define	ARPOP_RREQUEST	3		/* RARP request			*/
#define	ARPOP_RREPLY	4		/* RARP reply			*/

#define ARP_HTYPE_ETHER 1
#define ARP_PTYPE_IPV4 0x0800

#define CAPACITY 5

struct arp_hdr {
    uint16_t hwtype;
    uint16_t protype;
    unsigned char hwsize;
    unsigned char prosize;
    uint16_t opcode;
    unsigned char data[];
} __attribute__((packed));

typedef struct arp_hdr arp_hdr;

struct arp_ipv4 {
    unsigned char smac[6];
    uint32_t sip;
    unsigned char dmac[6];
    uint32_t dip;
} __attribute__((packed));

typedef struct arp_ipv4 arp_ipv4;

typedef struct table_entry {
    uint32_t key; // ip address
    unsigned char value[6]; // mac address
    uint32_t last_updated; //TODO: possibly change to time datatype
    struct table_entry *next;
} table_entry;

// hash table
typedef struct translation_table {
    table_entry **arr;
    int capacity;
    int size;
} translation_table;

static translation_table arp_table;

void set_entry(table_entry *node, uint32_t key, unsigned char value[6]);

void init_translation_table();

uint32_t hash_function(uint32_t key);

void insert(uint32_t key, unsigned char value[6]);

void delete_entry(uint32_t key);

table_entry *search(uint32_t key);

arp_hdr *parse_arp(unsigned char *buffer);

arp_ipv4 *parse_arp_ipv4(arp_hdr *arp_hdr);

void handle_arp(eth_hdr *eth, tun_device *device);

#ifdef __cplusplus
}
#endif

#endif //ARP_H
