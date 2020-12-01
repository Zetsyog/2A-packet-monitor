#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>

void parse_http(const unsigned char *packet, const uint16_t size) {
	BEGIN_LOG(COMPLETE);

	set_offset(3);
	log_title("HTTP Header");
	log_formatln("%-15s%hu", "Size", size);
	if (size) {
		log_formatln("- Data -");
		log_buf(packet, size);
	}

	END_LOG();

	BEGIN_LOG(SYNTH);
	set_offset(3);
	log_formatln("HTTP Size = %hu", size);

	END_LOG();
}