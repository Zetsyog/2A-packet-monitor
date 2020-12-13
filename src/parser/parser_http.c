#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>

void parse_http(const unsigned char *packet, const uint16_t size) {
	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);

	if (size) {
		set_offset(3);
		log_title("HTTP Header");
		log_buf(packet, size);
	}

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);
	set_offset(3);
	log_formatln("HTTP Size = %hu", size);
}