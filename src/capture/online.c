#include "capture/online.h"
#include "options.h"
#include "util.h"
#include "capture/common.h"
#include <pcap.h>
#include <stdlib.h>
#include <string.h>

void run_online() {
	printf("%s\n", pcap_lib_version());

	pcap_if_t *interfaces, *tmp;

	PCHECK(pcap_findalldevs(&interfaces, errbuf));

	int found = 0;

	for (tmp = interfaces; tmp; tmp = tmp->next) {
		if (!strcmp(tmp->name, options.name)) {
			found = 1;
			break;
		}
	}

	if (found == 0) {
		raler("Error: interface not found");
	}

	PCHECK2(handle = pcap_create(options.name, errbuf));

	if(pcap_activate(handle) < 0) {
		pcap_perror(handle, "pcap activate error");
		pcap_close(handle);
		exit(EXIT_FAILURE);
	}
	start_capture();
}
