#include "logger.h"
#include "parser.h"
#include <inttypes.h>
#include <netinet/ip.h>

#define TCP 0x06
#define UDP 0x11

void parse_ip(const unsigned char *packet) {
	struct iphdr *hdr = (struct iphdr *)packet;
	unsigned int protocol = hdr->protocol;
	char *protocol_str =
		protocol == TCP ? "TCP" : protocol == UDP ? "UDP" : "unsupported";

	uint32_t daddr = hdr->daddr, saddr = hdr->saddr;
	uint16_t frag_off = ntohs(hdr->frag_off);
	uint16_t tot_len = ntohs(hdr->tot_len);
	uint16_t ihl = hdr->ihl;

	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);

	set_offset(1);
	log_title("IP header");

	log_formatln("%-15s%i", "Version", hdr->version);
	log_formatln("%-15s%i", "IHL", ihl);
	log_formatln("%-15s0x%04x", "TOS", ntohs(hdr->tos));
	log_formatln("%-15s%d", "Total length", tot_len);
	log_formatln("%-15s0x%04x", "Id", ntohs(hdr->id));

	log_offset();
	log_format("%-15s%s", "Flags", "[");
	if (frag_off & 1 << 14)
		log_format(" DF");
	if (frag_off & 1 << 13)
		log_format(" MF");
	log_format(" ]\n");

	frag_off &= ~(0b111 << 12);

	log_formatln("%-15s%i", "Frag off", frag_off);
	log_formatln("%-15s%i", "TTL", hdr->ttl);
	log_formatln("%-15s%s", "Protocol", protocol_str);
	log_formatln("%-15s%04x", "CHKSUM", ntohs(hdr->check));

	log_offset();
	log_format("%-15s", "Source Addr");
	log_addr(saddr);
	log_format("\n");

	log_offset();
	log_format("%-15s", "Dest Addr");
	log_addr(daddr);
	log_format("\n");

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);

	set_offset(1);
	log_offset();
	log_format("IPv4, Src: ");
	log_addr(saddr);
	log_format(" > Dst: ");
	log_addr(daddr);
	log_format("\n");

	/**
	 * CONCISE Verbosity
	 */
	set_verbosity(CONCISE);
	log_format("IP Src: ");
	log_addr(saddr);
	log_format(", Dst: ");
	log_addr(daddr);

	switch (protocol) {
	case TCP:
		parse_tcp(packet + ihl * 4, tot_len - ihl * 4);
		break;
	case UDP:
		parse_udp(packet + ihl * 4);
		break;
	default:
		log_formatln("\tProtocol : unsupported");
		break;
	}
}