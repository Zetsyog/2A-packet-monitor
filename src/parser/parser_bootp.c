#include "logger.h"
#include "parser.h"
#include "parser/bootp.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

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
	log_formatln("%-15s%s", "Your Addr", inet_ntoa(hdr->ciaddr));
	log_formatln("%-15s%s", "Client Addr", inet_ntoa(hdr->yiaddr));
	log_formatln("%-15s%s", "Server Addr", inet_ntoa(hdr->siaddr));
	log_formatln("%-15s%s", "Gateway Addr", inet_ntoa(hdr->giaddr));

	log_offset();
	log_format("%-15s", "Client HW Addr");
	log_format("%02x", ntohs(hdr->chaddr[0]));
	for (int i = 1; i < 16; i++) {
		log_format(":%02x", ntohs(hdr->chaddr[i]));
	}
	log_format("\n");

	log_formatln("%-15s%s", "Server name", hdr->sname);

	log_formatln("%-15s%s", "Bootfile", hdr->file);

	// magic dhcp cookie
	if (hdr->vend[0] == 99 && hdr->vend[1] == 130 && hdr->vend[2] == 83 &&
		hdr->vend[3] == 99) {
		log_formatln("- DHCP Options -");
		int i = 4;
		struct bootp_option *bopt = (struct bootp_option *)(hdr->vend + i);
		while (1) {
			if (bopt->code == TAG_END) {
				break;
			}
			log_offset();
			log_format("%-30s", bootp_options_name[bopt->code].name);
			if (bopt->code == TAG_DHCP_MESSAGE) {
				log_format("%s", dhcp_messages[bopt->data[0]]);
			} else if (bopt->code == TAG_SERVER_SIP) {
				if(bopt->data[0] == 1) { // IPV4
					log_addr(ntohl(*((u_int32_t *)(bopt->data + 1))));
				}
			} else {
				switch (bootp_options_name[bopt->code].displaytype) {
				case PRINT_STR:
					fwrite(bopt->data, bopt->len, 1, stdout);
					break;
				case PRINT_IP:
					for (int j = 0; j < bopt->len; j += 4) {
						log_addr(ntohl(*((u_int32_t *)bopt->data)));
						log_format(" ");
					}
					break;
				case PRINT_UINT_32:
					log_format("%u", ntohl(*((u_int32_t *)bopt->data)));
					break;
				default:
					for (int j = 0; j < bopt->len; j++) {
						log_format("%hu", bopt->data[j]);
					}
					break;
				}
			}
			log_format("\n");

			i += bopt->len + 2;

			bopt = (struct bootp_option *)(hdr->vend + i);
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