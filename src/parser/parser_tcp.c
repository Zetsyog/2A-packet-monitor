#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define HTTP 80
#define SMTP 25
#define TELNET 23
#define FTP 21

#define CHECK_PROTO(x) source == x || dest == x

void parse_tcp(const unsigned char *packet, uint16_t size) {
	struct tcphdr *hdr = (struct tcphdr *)packet;
	uint16_t source = ntohs(hdr->source), dest = ntohs(hdr->dest),
			 doff = hdr->doff;

	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);

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

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);

	set_offset(2);
	log_formatln("TCP %hu > %hu", source, dest);

	uint16_t payload_size = size - doff * 4;
	const unsigned char *next = packet + doff * 4;
	if (payload_size == 0)
		return;

	if (CHECK_PROTO(HTTP)) {
		parse_http(next, payload_size);
	} else if (CHECK_PROTO(SMTP)) {
		parse_smtp(next, payload_size);
	} else if (CHECK_PROTO(TELNET)) {
		parse_telnet(next, payload_size);
	} else if (CHECK_PROTO(FTP)) {
		parse_ftp(next, payload_size);
	}
}