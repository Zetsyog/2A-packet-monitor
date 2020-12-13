#ifndef LOGGER_H
#define LOGGER_H

#include <netinet/in.h>
#include <stdint.h>

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define YELLOW "\033[0;33m"
#define BOLD_YELLOW "\033[01;33m"
#define BLUE "\033[0;34m"
#define BOLD_BLUE "\033[1;34m"
#define MAGENTA "\033[0;35m"
#define BOLD_MAGENTA "\033[1;35m"
#define CYAN "\033[0;36m"
#define BOLD_CYAN "\033[1;36m"
#define RESET "\033[0m"

#define BEGIN_LOG(verb)      \
	do {                     \
		set_verbosity(verb); \
	} while (0)
#define END_LOG() \
	do {          \
	} while (0)

enum LEVEL { CONCISE = 1, SYNTH = 2, COMPLETE = 3 };

void set_verbosity(int level);
void set_max_verbosity(int level);
void set_offset(unsigned int offset);
void log_offset();
void log_format(const char *message, ...);
void log_formatln(const char *message, ...);
void log_addr(uint32_t addr);
void log_addr6(struct in6_addr addr);
void log_error(const char *message, ...);
void log_buf(const unsigned char *buf, uint16_t);
void log_buf_offset(const unsigned char *buf, uint16_t);
void log_title(const char *title);
#endif