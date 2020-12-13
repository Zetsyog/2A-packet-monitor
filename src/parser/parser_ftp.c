#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>

void parse_ftp(const unsigned char *packet, const uint16_t size) {
	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);

	if (size) {
		set_offset(3);
		log_title("FTP Header");
		log_buf_offset(packet, size);
	}

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);
	set_offset(3);
	log_formatln("FTP Size = %hu", size);
}