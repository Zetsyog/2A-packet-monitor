#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include "parser/bootp.h"

void parse_bootp(const unsigned char *packet) {
    struct bootp_hdr *hdr = (struct bootp_hdr *)packet;

	BEGIN_LOG(COMPLETE);

	set_offset(2);
	log_formatln("- Bootp Header -");

    log_formatln("%-15s%hu", "Operation :", ntohs(hdr->op));
	log_formatln("%-15s%hu", "Hw type :", ntohs(hdr->htype));
	log_formatln("%-15s%hu", "Hw addr len :", ntohs(hdr->hlen));
	log_formatln("%-15s%hu", "Hop count :", ntohs(hdr->hops));
	log_formatln("%-15s%u", "Transx ID :", ntohl(hdr->xid));
	log_formatln("%-15s%hu", "# of sec :", ntohs(hdr->secs));
	log_formatln("%-15s%hu", "Flags : ", ntohs(hdr->hops) != 0 ? "[B]" : "[]");
	
    log_offset();
	log_format("%-15s", "Client Addr :");
	log_addr(ntohl(hdr->ciaddr));
	log_format("\n");

    log_offset();
	log_format("%-15s", "Your Addr :");
	log_addr(ntohl(hdr->yiaddr));
	log_format("\n");

    log_offset();
	log_format("%-15s", "Server Addr :");
	log_addr(ntohl(hdr->siaddr));
	log_format("\n");

    log_offset();
	log_format("%-15s", "Gateway Addr :");
	log_addr(ntohl(hdr->giaddr));
	log_format("\n");

    log_offset();
    log_format("%02x", ntohs(hdr->chaddr[0]));
	for (int i = 1; i < 16; i++) {
		log_format(":%02x", ntohs(hdr->chaddr[i]));
	}
	log_format("\n");

	log_formatln("%-15s%s", "Server name : ", hdr->server_name);
	log_formatln("%-15s%s", "Bootfile : ", hdr->boot_file);

	END_LOG();

	BEGIN_LOG(SYNTH);

	set_offset(3);

	END_LOG();
}