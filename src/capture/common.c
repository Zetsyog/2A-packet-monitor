#include "capture/common.h"
#include "options.h"
#include "parser.h"
#include "signal.h"
#include "util.h"
#include <pcap/pcap.h>
#include <stdlib.h>

void intHandler(int dummy) {
	(void)dummy;

	pcap_breakloop(handle);
}

void got_packet(u_char *user, const struct pcap_pkthdr *h,
				const u_char *bytes) {	
	(void)user;

	static time_t first_ts = 0;

	if(!first_ts)
		first_ts = h->ts.tv_sec;

	printf("\nFRAME Time: %li.%li Length: %i bytes", h->ts.tv_sec - first_ts, h->ts.tv_usec, h->len);
	
	if(options.verbose_level >= SYNTH)
		printf("\n");

	// printf("==== Got a %d byte packet ====\n", h->len);
	parse_ethernet(bytes);
}

void compile_filter(struct bpf_program *fp) {
	if (pcap_compile(handle, fp, options.filter, 0,
						PCAP_NETMASK_UNKNOWN) == PCAP_ERROR) {
		pcap_perror(handle, "pcap filter compile");
		pcap_close(handle);
		exit(EXIT_FAILURE);
	}

	if (pcap_setfilter(handle, fp) == PCAP_ERROR) {
		pcap_perror(handle, "pcap set filter");
		pcap_freecode(fp);
		pcap_close(handle);
		exit(EXIT_FAILURE);
	}
}

void start_capture() {
	signal(SIGINT, intHandler);

	struct bpf_program fp;
	compile_filter(&fp);
	pcap_setfilter(handle, &fp);

	int ret = pcap_loop(handle, 0, got_packet, NULL);


	if (ret == PCAP_ERROR || ret == PCAP_ERROR_BREAK) {
		pcap_perror(handle, "pcap loop");
	}

	pcap_freecode(&fp);
	pcap_close(handle);
}