#include "capture/common.h"
#include "util.h"
#include "signal.h"

static int keepRunning = 1;

void intHandler(int dummy) {
	keepRunning = 0;
}

void got_packet(u_char *user, const struct pcap_pkthdr *h,
				const u_char *bytes) {
	printf("New paquet \n");
	int count = 0;
	for (unsigned int i = 0; i < h->caplen; i++) {
		printf("%x ", bytes[i]);
		count++;
		if (count >= 16) {
			count = 0;
			printf("\n");
		}
	}
}

void start_capture() {
	if(pcap_loop(handle, -1, got_packet, NULL) == PCAP_ERROR) {

	};
	signal(SIGINT, intHandler);
	while(keepRunning)
		pcap_breakloop(handle);

	pcap_close(handle);
}

void destroy() {

}