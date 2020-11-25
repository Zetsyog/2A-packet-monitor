#include "capture/offline.h"
#include "capture/common.h"
#include "options.h"
#include "util.h"
#include <pcap.h>
#include <stdlib.h>
#include <string.h>

void run_offline() {

	printf("%s\n", pcap_lib_version());

	char errbuf[PCAP_ERRBUF_SIZE];

	if ((handle = pcap_open_offline(options.name, errbuf)) == NULL) {
		PRINT_ERRBUF();
		exit(EXIT_FAILURE);
	}

	start_capture();
}
