#ifndef LOGGER_H
#define LOGGER_H

#include <sys/types.h>

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

#define BEGIN_LOG(verb) if (verbosity == verb) {
#define END_LOG() }

enum { CONCISE = 1, SYNTH = 2, COMPLETE = 3 } verbosity;

void set_verbosity(int level);
void set_offset(unsigned int offset);
void log_offset();
void log_format(const char *message, ...);
void log_formatln(const char *message, ...);
void log_addr(u_int32_t addr);
void log_error(const char *message, ...);
#endif