#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <netinet/udp.h>

#define BOOTP_SERVER_PORT 67
#define BOOTP_CLIENT_PORT 68

void parse_udp(const unsigned char *packet) {
	struct udphdr *hdr = (struct udphdr *)packet;

	uint16_t source = ntohs(hdr->source);
	uint16_t dest = ntohs(hdr->dest);
	BEGIN_LOG(COMPLETE);

	set_offset(2);
	log_title("UDP Header");

	log_formatln("%-15s%hu", "Source Port :", source);
	log_formatln("%-15s%hu", "Dest Port :", dest);
	log_formatln("%-15s%u", "Chk :", ntohs(hdr->check));
	log_formatln("%-15s%u", "Len :", ntohs(hdr->len));

	END_LOG();

	BEGIN_LOG(SYNTH);

	set_offset(2);
	log_formatln("UDP %hu > %hu", source, dest);

	END_LOG();

	if (dest == BOOTP_CLIENT_PORT || source == BOOTP_SERVER_PORT) {
        parse_bootp(packet + sizeof(struct udphdr));
    }
}