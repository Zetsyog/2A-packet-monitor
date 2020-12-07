#include "logger.h"
#include "parser.h"
#include <arpa/inet.h>
#include <unistd.h>

void parse_ftp(const unsigned char *packet, const uint16_t size) {
	BEGIN_LOG(COMPLETE);

	if (size) {
	    set_offset(3);        
	    log_title("FTP Header");
		log_buf(packet, size);
	}

	END_LOG();

	BEGIN_LOG(SYNTH);
	set_offset(3);
	log_formatln("FTP Size = %hu", size);

	END_LOG();
}