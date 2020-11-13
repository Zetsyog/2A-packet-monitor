#include "logger.h"
#include "options.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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