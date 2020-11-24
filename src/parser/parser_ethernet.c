#include "logger.h"
#include "parser.h"
#include <netinet/if_ether.h>
#include <arpa/inet.h>


static void log_mac_addr(unsigned char *addr) {
	log_format("%02x", addr[0]);
	for (int i = 1; i < ETH_ALEN; i++) {
		log_format(":%02x", addr[i]);
	}
}

void parse_ethernet(const unsigned char *packet) {
	struct ethhdr *hdr = (struct ethhdr *)packet;
    unsigned short proto = ntohs(hdr->h_proto);
    char *proto_str = proto == ETH_P_IP ? "IP" : proto == ETH_P_ARP ? "ARP" : "unsupported";

    BEGIN_LOG(COMPLETE);

    set_offset(0);
	log_formatln("- Ethernet Header -");
	log_format("Destination : ");
	log_mac_addr(hdr->h_dest);
	log_formatln("");

	log_format("Source : ");
	log_mac_addr(hdr->h_source);
	log_formatln("");

    log_formatln("Type : %s", proto_str);

    END_LOG();

    BEGIN_LOG(SYNTH);

    set_offset(0);
    log_format("ETH ");
    log_mac_addr(hdr->h_source);
    log_format(" > ");
    log_mac_addr(hdr->h_dest);
    log_format("\n");

    END_LOG();
    
	switch (proto) {
	case ETH_P_IP:
        parse_ip(packet + sizeof(struct ethhdr));
		break;
    case ETH_P_ARP:
        break;
	default:
		break;
	}
}
