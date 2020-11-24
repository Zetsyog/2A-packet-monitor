#include "logger.h"
#include "parser.h"
#include "parser/bootp.h"
#include <arpa/inet.h>
#include <netinet/udp.h>

void parse_udp(const unsigned char *packet) {
	struct udphdr *hdr = (struct udphdr *)packet;

	uint16_t source = ntohs(hdr->source);
	uint16_t dest = ntohs(hdr->dest);
	BEGIN_LOG(COMPLETE);

	set_offset(2);
	log_formatln("- UDP Header -");

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