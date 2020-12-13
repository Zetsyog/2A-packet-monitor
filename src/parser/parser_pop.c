#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

void parse_pop(const unsigned char *packet, const uint16_t size) {
    // Check if it really pop proto
    if(!isprint(packet[0])) {
        return;
    }

	/**
	 * COMPLETE Verbosity
	 */
	set_verbosity(COMPLETE);

	if (size) {
		set_offset(3);
		log_title("POP");
		log_buf_offset(packet, size);
	}

	/**
	 * SYNTH Verbosity
	 */
	set_verbosity(SYNTH);
	set_offset(3);
	log_formatln("Post Office Protocol Size = %hu", size);

	/**
	 * CONCISE Verbosity
	 */
	set_verbosity(CONCISE);
	log_format(", POP");
}