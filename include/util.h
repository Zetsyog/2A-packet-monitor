#include <pcap.h>

#ifndef UTIL_H
#define UTIL_H

#define PRINT_ERRBUF(error) raler("%s: %s", #error, errbuf)

#define PCHECK(op)             \
	do {                       \
		if ((op) == -1)        \
			PRINT_ERRBUF(#op); \
	} while (0)

#define PCHECK2(op)            \
	do {                       \
		if ((op) == NULL)      \
			PRINT_ERRBUF(#op); \
	} while (0)

char errbuf[PCAP_ERRBUF_SIZE];

void raler(const char *msg, ...);

#endif