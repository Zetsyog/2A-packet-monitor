#include "capture/online.h"
#include "capture/common.h"
#include "options.h"
#include "util.h"
#include <pcap.h>
#include <stdlib.h>
#include <string.h>

void run_online() {

	printf("%s\n", pcap_lib_version());

	char errbuf[PCAP_ERRBUF_SIZE];

	if ((handle = pcap_create(options.name, errbuf)) == NULL) {
		PRINT_ERRBUF();
		exit(EXIT_FAILURE);
	}	

	pcap_set_timeout(handle, PCAP_TIMEOUT);

	int status;
	if ((status = pcap_activate(handle)) < 0) {
		pcap_perror(handle, "pcap activate error");
		pcap_close(handle);
		exit(EXIT_FAILURE);
	}

	start_capture();
}
