#include "logger.h"
#include "parser.h"
#include <net/if_arp.h>

static void print_addr(uint16_t len, const uint8_t *payload) {
	if (len == 4) { // Ipv4
		log_addr(*((uint32_t *)payload));
	} else if (len == 16) { // Ipv6
		log_addr6(*((struct in6_addr *)payload));
	}
}

static void print_mac(uint16_t len, const uint8_t *payload) {
    if (len == 6) { // Mac addr
		log_format("%02x:%02x:%02x:%02x:%02x:%02x", payload[0], payload[1],
				   payload[2], payload[3], payload[4], payload[5]);
	} else if (len == 1) {
		log_format("%02x (token ring)", payload[0]);
	}
}

void parse_arp(const unsigned char *packet) {
	struct arphdr *hdr = (struct arphdr *)packet;
	uint16_t op = ntohs(hdr->ar_op);
    char *title = op == ARPOP_RREPLY || op == ARPOP_RREQUEST ? "RARP" : "ARP";
	char *op_str = op == ARPOP_REQUEST
					   ? "request"
					   : op == ARPOP_REPLY ? "reply" 
                       : op == ARPOP_RREQUEST ? "reverse request"
                       : op == ARPOP_RREPLY ? "reverse reply" 
                       : "unknown";

	/**
	 * COMPLETE verbosity
	 */
	set_verbosity(COMPLETE);
	set_offset(1);
	log_title(title);

	log_formatln("%-20s0x%04x", "Hw type", ntohs(hdr->ar_hrd));
	log_formatln("%-20s0x%04x", "Protocol type", ntohs(hdr->ar_pro));
	log_formatln("%-20s%u", "Hw addr len", hdr->ar_hln);
	log_formatln("%-20s%u", "Hw proto len", hdr->ar_pln);
	log_formatln("%-20s%u (%s)", "Opcode", op, op_str);

	const uint8_t *payload = packet + sizeof(struct arphdr);

	log_offset();
	log_format("%-20s", "Sender hw addr");
	print_mac(hdr->ar_hln, payload);
	log_format("\n");
	payload += hdr->ar_hln;

	log_offset();
	log_format("%-20s", "Sender proto addr");
	print_addr(hdr->ar_pln, payload);
	log_format("\n");
	payload += hdr->ar_pln;

	log_offset();
	log_format("%-20s", "Target hw addr");
	print_mac(hdr->ar_hln, payload);
	log_format("\n");
	payload += hdr->ar_hln;

	log_offset();
	log_format("%-20s", "Target proto addr");
	print_addr(hdr->ar_pln, payload);
	log_format("\n");
	payload += hdr->ar_pln;

	payload = packet + sizeof(struct arphdr);

	/**
	 * SYNTH verbosity
	 */
	set_verbosity(SYNTH);
	set_offset(1);
    log_offset();
	log_format("Address Resolution Protocol (%s) ", op_str);
    if (op == ARPOP_REQUEST) {
		log_format("Who has ");
		print_addr(hdr->ar_pln, payload + hdr->ar_hln * 2 + hdr->ar_pln);
		log_format(" ? Tell ");
        print_addr(hdr->ar_pln, payload + hdr->ar_hln);
	} else if(op == ARPOP_REPLY) {
		print_addr(hdr->ar_pln, payload + hdr->ar_hln);
		log_format(" is at ");
        print_mac(hdr->ar_hln, payload + hdr->ar_hln + hdr->ar_pln);
    }
    log_format("\n");

	/**
	 * CONCISE verbosity
	 */
	set_verbosity(CONCISE);

	log_format("%s (%s) ", title, op_str);
	if (op == ARPOP_REQUEST) {
		log_format("Who has ");
		print_addr(hdr->ar_pln, payload + hdr->ar_hln * 2 + hdr->ar_pln);
		log_format(" ? Tell ");
        print_addr(hdr->ar_pln, payload + hdr->ar_hln);
	} else if(op == ARPOP_REPLY) {
		print_addr(hdr->ar_pln, payload + hdr->ar_hln);
		log_format(" is at ");
        print_mac(hdr->ar_hln, payload + hdr->ar_hln + hdr->ar_pln);
    } else if (op == ARPOP_RREQUEST) {
		log_format("Who is ");
		print_mac(hdr->ar_hln, payload + hdr->ar_hln + hdr->ar_pln);
		log_format(" ? Tell ");
        print_mac(hdr->ar_hln, payload);
	} else if(op == ARPOP_RREPLY) {
		print_mac(hdr->ar_hln, payload + hdr->ar_hln + hdr->ar_pln);
		log_format(" is at ");
        print_addr(hdr->ar_pln, payload + hdr->ar_hln * 2 + hdr->ar_pln);
    }
}