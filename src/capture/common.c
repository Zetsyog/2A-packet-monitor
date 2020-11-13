#include "capture/common.h"
#include "signal.h"
#include "parser.h"
#include "util.h"

void intHandler(int dummy) {
	(void) dummy;

	pcap_breakloop(handle);
}

void got_packet(u_char *user, const struct pcap_pkthdr *h,
				const u_char *bytes) {
	(void) user;

	// printf("==== Got a %d byte packet ====\n", h->len);
	parse_ethernet(bytes);
}

void start_capture() {
	signal(SIGINT, intHandler);

	int ret = pcap_loop(handle, 0, got_packet, NULL);

	if (ret == PCAP_ERROR || ret == PCAP_ERROR_BREAK) {
		pcap_perror(handle, "pcap loop");
	}

	pcap_close(handle);
}

void destroy() {
}