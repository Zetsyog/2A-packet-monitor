#include "logger.h"
#include "options.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void log_addr(u_int32_t addr) {
	unsigned char bytes[4];
	bytes[0] = addr & 0xFF;
	bytes[1] = (addr >> 8) & 0xFF;
	bytes[2] = (addr >> 16) & 0xFF;
	bytes[3] = (addr >> 24) & 0xFF;
	log_format("%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
}

void log_buf(const unsigned char *buf) {
	char * line = strtok((char *) buf, "\n");
	while(line) {
		log_offset();
		log_format("%s\n", line);
		line  = strtok(NULL, "\n");
	}
}

void log_title(const char *title) {
	printf(RESET);
	log_formatln("--- %s ---", title);
}