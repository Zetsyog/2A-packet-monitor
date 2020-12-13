#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <netinet/if_ether.h>

static void log_mac_addr(unsigned char *addr) {
	log_format("%02x", addr[0]);
	for (int i = 1; i < ETH_ALEN; i++) {
		log_format(":%02x", addr[i]);
	}
}

void parse_ethernet(const unsigned char *packet) {
	struct ethhdr *hdr = (struct ethhdr *)packet;
	unsigned short proto = ntohs(hdr->h_proto);
	char *proto_str = "unsupported";
	if (proto == ETH_P_IP)
		proto_str = "IPv4";
	if (proto == ETH_P_IPV6)
		proto_str = "IPv6";
	if(proto == ETH_P_ARP)
		proto_str = "ARP";
	if(proto == ETH_P_RARP)
		proto_str = "RARP";

	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);

	set_offset(0);
	log_title("Ethernet Header");

	log_format("%-15s", "Source");
	log_mac_addr(hdr->h_source);
	log_formatln("");

	log_format("%-15s", "Destination");
	log_mac_addr(hdr->h_dest);
	log_formatln("");

	log_formatln("%-15s%s", "Type", proto_str);

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);

	set_offset(0);
	log_format("Ethernet, Src: ");
	log_mac_addr(hdr->h_source);
	log_format(" > Dst: ");
	log_mac_addr(hdr->h_dest);
	log_format("\n");

	switch (proto) {
	case ETH_P_IP:
		parse_ip(packet + sizeof(struct ethhdr));
		break;
	case ETH_P_IPV6:
		parse_ipv6(packet + sizeof(struct ethhdr));
		break;
	case ETH_P_ARP:
	case ETH_P_RARP:
		parse_arp(packet + sizeof(struct ethhdr));
		break;
	
	default:
		break;
	}
}
