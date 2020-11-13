#include "parser.h"
#include <arpa/inet.h>
#include "logger.h"

void parse_http(const unsigned char *packet){
    BEGIN_LOG(COMPLETE);

    set_offset(3);
    log_formatln("HTTP");
    log_formatln("%s", packet);

    END_LOG();

    BEGIN_LOG(SYNTH);
    set_offset(3);
    log_formatln("HTTP");
    log_formatln("%s", packet);

    END_LOG();
}