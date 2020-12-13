#include "parser.h"
#include <net/if_arp.h>

void parse_arp(const unsigned char *packet) {
    struct arphdr *hdr = (struct arphdr *) packet;
    
}