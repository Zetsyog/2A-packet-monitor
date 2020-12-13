#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>

void parse_imap(const unsigned char *packet, const uint16_t size) {
	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);
	set_offset(3);

	log_title("IMAP");
	log_formatln("%-15s%hu", "Size", size);
	if (size) {
		log_formatln("- Data -");
		log_buf_offset(packet, size);
	}

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);
	set_offset(3);
	log_formatln("Internet Message Access Protocol, size: %hu", size);

	/**
	 * CONCISE Verbosity
	 */
	set_verbosity(CONCISE);
	log_format(", IMAP");
}