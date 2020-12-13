#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <netinet/udp.h>

#define BOOTP_SERVER_PORT 67
#define BOOTP_CLIENT_PORT 68
#define DNS_PORT 53

void parse_udp(const unsigned char *packet) {
	struct udphdr *hdr = (struct udphdr *)packet;

	uint16_t source = ntohs(hdr->source);
	uint16_t dest = ntohs(hdr->dest);

	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);
	set_offset(2);
	log_title("UDP Header");

	log_formatln("%-15s%hu", "Source Port :", source);
	log_formatln("%-15s%hu", "Dest Port :", dest);
	log_formatln("%-15s%u", "Chk :", ntohs(hdr->check));
	log_formatln("%-15s%u", "Len :", ntohs(hdr->len));

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);
	set_offset(2);
	log_formatln("Udp, Src port: %hu > Dst port: %hu", source, dest);

	if (dest == BOOTP_CLIENT_PORT || source == BOOTP_SERVER_PORT) {
		parse_bootp(packet + sizeof(struct udphdr));
	} else if (dest == DNS_PORT || source == DNS_PORT) {
		parse_dns(packet + sizeof(struct udphdr));
	} else {
	}
}