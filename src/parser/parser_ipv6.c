#include "logger.h"
#include "parser.h"
#include <inttypes.h>
#include <netinet/ip6.h>

#define TCP 0x06
#define UDP 0x11

#define HEADER_SIZE 40

struct ip6_flow_t {
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int traffic_class_hi : 4, version : 4;
	unsigned int flow_label_hi : 4, traffic_class_lo : 4;
	uint16_t flow_label_lo;
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned int version : 4, traffic_class_hi : 4;
	unsigned int traffic_class_lo : 4, flow_label_hi : 4;
	uint16_t flow_label_lo;
#else
#error "Please fix <bits/endian.h>"
#endif
};

void parse_ipv6(const unsigned char *packet) {
	struct ip6_hdr *hdr = (struct ip6_hdr *)packet;
	uint8_t protocol = hdr->ip6_ctlun.ip6_un1.ip6_un1_nxt;
	char *protocol_str =
		protocol == TCP ? "TCP" : protocol == UDP ? "UDP" : "unsupported";

	uint16_t payload_len = ntohs(hdr->ip6_ctlun.ip6_un1.ip6_un1_plen);

	struct in6_addr daddr = hdr->ip6_dst, saddr = hdr->ip6_src;

	struct ip6_flow_t *flow =
		(struct ip6_flow_t *)&(hdr->ip6_ctlun.ip6_un1.ip6_un1_flow);

	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);

	set_offset(1);
	log_title("IPv6 header");
	log_formatln("%-15s%i", "Version : ", flow->version);
	log_formatln("%-15s0x%01x%01x", "Traffic Class", flow->traffic_class_hi,
				 flow->traffic_class_lo);
	log_formatln("%-15s0x%02x%04x", "Flow Label", flow->flow_label_hi,
				 flow->flow_label_lo);
	log_formatln("%-15s%u", "Payload len", payload_len);
	log_formatln("%-15s%s", "Next Header", protocol_str);
	log_formatln("%-15s%d", "Hop limit", hdr->ip6_ctlun.ip6_un1.ip6_un1_hlim);

	log_offset();
	log_format("%-15s", "Source Addr");
	log_addr6(saddr);
	log_format("\n");

	log_offset();
	log_format("%-15s", "Dest Addr");
	log_addr6(daddr);
	log_format("\n");

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);

	set_offset(1);
	log_offset();
	log_format("IPv6 ");
	log_addr6(saddr);
	log_format(" > ");
	log_addr6(daddr);
	log_format("\n");

	switch (protocol) {
	case TCP:
		parse_tcp(packet + HEADER_SIZE, payload_len);
		break;
	case UDP:
		parse_udp(packet + HEADER_SIZE);
		break;
	default:
		log_formatln("\tProtocol : unsupported");
		break;
	}
}