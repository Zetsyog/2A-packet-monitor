#include "logger.h"
#include "options.h"
#include <bits/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static int offset = 0;

void set_verbosity(int level) {
	verbosity = level;
}

void set_offset(unsigned int i) {
	offset = i;
}

void log_offset() {

	for (int i = 0; i < offset; i++) {
		printf("\t");
	}
}

void log_format(const char *message, ...) {
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
}

void log_formatln(const char *message, ...) {
	log_offset();

	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	printf("\n");
}

void log_error(const char *message, ...) {
	printf(BOLD_RED);
	printf("Error : ");
	printf(RESET);

	printf(RED);
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);

	printf(RESET);
	printf("\n");
}

void log_addr(uint32_t addr) {
	char str[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN);
	log_format("%s", str);
}

void log_addr6(struct in6_addr addr) {
	char str[INET6_ADDRSTRLEN]; // space to hold the IPv6 string

	inet_ntop(AF_INET6, &addr, str, INET6_ADDRSTRLEN);

	log_format("%s", str);
}

void log_buf(const unsigned char *buf, uint16_t size) {
	int write = 0;
	log_offset();
	for (int i = 0; i < size; i++) {
		if(buf[i] == 0) break;
		switch (buf[i]) {
		case '\r':
			printf("\\r");
			break;
		case '\n':
			printf("\\n\n");
			log_offset();
			break;
		default:
			printf("%c", buf[i]);
			break;
		}
		write++;
	}
	printf("\n");
}

void log_title(const char *title) {
	printf(RESET);
	log_formatln("--- %s ---", title);
}