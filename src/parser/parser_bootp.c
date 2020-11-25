#include "logger.h"
#include "parser.h"
#include "parser/bootp.h"
#include <arpa/inet.h>

void parse_bootp(const unsigned char *packet) {
	struct bootp_hdr *hdr = (struct bootp_hdr *)packet;

	BEGIN_LOG(COMPLETE);

	set_offset(3);
	log_title("Bootp Header");

	log_formatln("%-15s%hu", "Operation", ntohs(hdr->op));
	log_formatln("%-15s%hu", "Hw type", ntohs(hdr->htype));
	log_formatln("%-15s%hu", "Hw addr len", ntohs(hdr->hlen));
	log_formatln("%-15s%hu", "Hop count", ntohs(hdr->hops));
	log_formatln("%-15s%u", "Transx ID", ntohl(hdr->xid));
	log_formatln("%-15s%hu", "# of sec", ntohs(hdr->secs));
	log_formatln("%-15s%hu", "Flags", ntohs(hdr->hops) != 0 ? "[B]" : "[]");

	log_offset();
	log_format("%-15s", "Client Addr");
	log_addr(ntohl(hdr->ciaddr));
	log_format("\n");

	log_offset();
	log_format("%-15s", "Your Addr");
	log_addr(ntohl(hdr->yiaddr));
	log_format("\n");

	log_offset();
	log_format("%-15s", "Server Addr");
	log_addr(ntohl(hdr->siaddr));
	log_format("\n");

	log_offset();
	log_format("%-15s", "Gateway Addr");
	log_addr(ntohl(hdr->giaddr));
	log_format("\n");

	log_offset();
	log_format("%-15s", "Client HW Addr");
	log_format("%02x", ntohs(hdr->chaddr[0]));
	for (int i = 1; i < 16; i++) {
		log_format(":%02x", ntohs(hdr->chaddr[i]));
	}
	log_format("\n");

	log_formatln("%-15s%s", "Server name", hdr->server_name);

	log_formatln("%-15s%s", "Bootfile", hdr->boot_file);

	// magic dhcp cookie
	if (hdr->vendor[0] == 99 && hdr->vendor[1] == 130 && hdr->vendor[2] == 83 &&
		hdr->vendor[3] == 99) {
		log_formatln("- DHCP Options -");
		int i = 4;
		struct bootp_option *bopt = (struct bootp_option *)(hdr->vendor + i);
		while (1) {
			if (bopt->code == BOOTP_OPT_END) {
				break;
			}
			log_offset();
			log_format("%-30s", bootp_options_name[bopt->code]);
			if (bopt->code == BOOTP_OPT_DHCP_MSG) {
				log_format("%s", dhcp_messages[bopt->data[0]]);
			} else {
				for (int j = 0; j < bopt->len; j++) {
					log_format("%hu", bopt->data[j]);
				}
			}
			log_format("\n");

			i += bopt->len + 2;

			bopt = (struct bootp_option *)(hdr->vendor + i);
		}
	}

	END_LOG();

	BEGIN_LOG(SYNTH);

	set_offset(3);
	log_offset();
	log_format("BOOTP ");
	log_format("%s %hu", "Op.", ntohs(hdr->op));

	END_LOG();
}