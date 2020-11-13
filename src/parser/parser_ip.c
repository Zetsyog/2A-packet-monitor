#include "logger.h"
#include "parser.h"
#include <inttypes.h>
#include <netinet/ip.h>

#define TCP 0x06
#define UDP 0x11

static void log_addr(uint32_t addr) {
	unsigned char bytes[4];
	bytes[0] = addr & 0xFF;
	bytes[1] = (addr >> 8) & 0xFF;
	bytes[2] = (addr >> 16) & 0xFF;
	bytes[3] = (addr >> 24) & 0xFF;
	log_format("%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
}

void parse_ip(const unsigned char *packet) {
	struct iphdr *hdr = (struct iphdr *)packet;
	unsigned int protocol = hdr->protocol;
	char *protocol_str =
		protocol == TCP ? "TCP" : protocol == UDP ? "UDP" : "unsupported";
	uint32_t daddr = ntohl(hdr->daddr), saddr = ntohl(hdr->saddr);
	uint16_t frag_off = ntohs(hdr->frag_off);
    uint16_t tot_len = ntohs(hdr->tot_len);

	// Complete verbose display
	BEGIN_LOG(COMPLETE);

	set_offset(1);
	log_formatln("- IP header -");

	log_formatln("%-15s%i", "Version :", hdr->version);
	log_formatln("%-15s%i", "IHL", hdr->ihl);
	log_formatln("%-15s0x%04x", "TOS", ntohs(hdr->tos));
	log_formatln("%-15s%d", "Total length :", tot_len);
	log_formatln("%-15s0x%04x", "Id : ", ntohs(hdr->id));

	log_offset();
	log_format("%-15s%s", "Flags :", "[");
	if (frag_off & 1 << 14)
		log_format(" DF");
	if (frag_off & 1 << 13)
		log_format(" MF");
	log_format(" ]\n");

	frag_off &= ~(0b111 << 12);

	log_formatln("%-15s%i", "Frag off :", frag_off);
	log_formatln("%-15s%i", "TTL :", hdr->ttl);
	log_formatln("%-15s%s", "Protocol :", protocol_str);
	log_formatln("%-15s%04x", "CHKSUM", ntohs(hdr->check));

	log_offset();
	log_format("%-15s", "Source Addr :");
	log_addr(saddr);
	log_format("\n");

	log_offset();
	log_format("%-15s", "Dest Addr : ");
	log_addr(daddr);
	log_format("\n");

	END_LOG();

	// Synth verbodse
	BEGIN_LOG(SYNTH);

	set_offset(1);
	log_offset();
	log_format("IP ");
	log_addr(saddr);
	log_format(" > ");
	log_addr(daddr);
	log_format("\n");

	END_LOG();

	switch (protocol) {
	case TCP:
		parse_tcp(packet + hdr->ihl * 4);
		break;
	case UDP:
		break;
	default:
		log_formatln("\tProtocol : unsupported");
		break;
	}
}