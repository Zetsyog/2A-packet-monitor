#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>

#define TELCMDS
#define TELOPTS

#include <arpa/telnet.h>

void parse_cmd(const unsigned char *packet, uint16_t size) {
	for (int i = 0; i < size; i++) {
		if (packet[i] == IAC) {
			log_formatln("%-15s%s", TELCMD(packet[i+1]), TELOPT(packet[i + 2]));
			i+=2;
		}
	}
}

void parse_telnet(const unsigned char *packet, uint16_t size) {
	BEGIN_LOG(COMPLETE);

	set_offset(3);
	log_title("Telnet Header");
	log_formatln("%-15s%hu", "Size", size);
	if (size) {
		log_formatln("- Data -");
		if (*packet == IAC) {
			parse_cmd(packet, size);
		} else {
			log_buf(packet, size);
		}
	}

	END_LOG();

	BEGIN_LOG(SYNTH);
	set_offset(3);
	log_formatln("SMTP Size = %hu", size);

	END_LOG();
}