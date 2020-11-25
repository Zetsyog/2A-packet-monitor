#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define HTTP 80
#define SMTP 25

void parse_tcp(const unsigned char *packet, uint16_t size) {
	struct tcphdr *hdr = (struct tcphdr *)packet;
	uint16_t source = ntohs(hdr->source), dest = ntohs(hdr->dest),
			 doff = hdr->doff;

	BEGIN_LOG(COMPLETE);

	set_offset(2);
	log_title("TCP Header");

	log_formatln("%-15s%hu", "Source Port :", source);
	log_formatln("%-15s%hu", "Dest Port :", dest);
	log_formatln("%-15s%u", "Seq no. :", ntohl(hdr->seq));
	log_formatln("%-15s%u", "Ack no. :", ntohl(hdr->ack_seq));
	log_formatln("%-15s%hu", "Data off", doff);

	log_offset();
	log_format("%-15s[", "Flags");
	if (hdr->fin)
		log_format(" FIN");
	if (hdr->syn)
		log_format(" SYN");
	if (hdr->rst)
		log_format(" RST");
	if (hdr->psh)
		log_format(" PSH");
	if (hdr->ack)
		log_format(" ACK");
	if (hdr->urg)
		log_format(" URG");
	log_format(" ]\n");

	log_formatln("%-15s%hu", "Window", ntohs(hdr->window));
	log_formatln("%-15s0x%04x", "Checksum", ntohs(hdr->check));
	log_formatln("%-15s%hu", "Urgent ptr", ntohs(hdr->urg_ptr));

	END_LOG();

	BEGIN_LOG(SYNTH);

	set_offset(2);
	log_formatln("TCP %hu > %hu", source, dest);

	END_LOG();

	if(dest == HTTP || source == HTTP) {
		parse_http(packet + doff * 4, size - doff * 4);
	} else if(dest == SMTP || source == SMTP) {
		parse_smtp(packet + doff * 4, size - doff * 4);
	}
}