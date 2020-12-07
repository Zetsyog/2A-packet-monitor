#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>

#define TELCMDS
#define TELOPTS

#include <arpa/telnet.h>

static int telnet_opt(const unsigned char *packet) {
	if (*packet == SB) {
		log_format("%-15s%s\n", TELCMD(*packet), TELOPT(*(packet + 1)));
		log_offset();
		log_format("\t%-15s", "Option Data");
		packet += 2;
		int i = 2;
		while ((*packet) != IAC && (*(packet + 1)) != SE) {
			log_format("%u ", (unsigned int) *packet);
			i++;
			packet++;
		}
		i+=2;
		log_format("\n");
		log_formatln("Suboption end (240)");
		return i;
	} else {
		log_format("%-15s%s\n", TELCMD(*packet), TELOPT(*(packet + 1)));
	}
	return 2;
}

static void telnet_line(const unsigned char *packet, uint16_t size) {
	int newline = 1;
	for (int i = 0; i < size;) {
		if (newline) {
			log_offset();
			newline = 0;
		}
		switch (packet[i]) {
		case IAC:
			i++;
			i += telnet_opt(packet + i);
			newline++;
			break;
		case 0:
			log_format("NULL");
			i++;
			break;
		case '\n':
			log_format("\\n\n");
			newline++;
			i++;
			break;
		case '\r':
			log_format("\\r");
			i++;
			break;
		default:
			log_format("%c", packet[i]);
			i++;
			break;
		}
	}
	log_format("\n");
}

void parse_telnet(const unsigned char *packet, uint16_t size) {
	BEGIN_LOG(COMPLETE);

	set_offset(3);
	if (size) {
		log_title("Telnet Header");
		telnet_line(packet, size);
	}

	END_LOG();

	BEGIN_LOG(SYNTH);
	set_offset(3);
	log_formatln("SMTP Size = %hu", size);

	END_LOG();
}