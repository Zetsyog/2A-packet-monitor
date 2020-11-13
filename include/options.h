#ifndef OPTIONS_H
#define OPTIONS_H

#include "logger.h"

struct options {
	enum EXEC_TYPE { NONE, ONLINE, OFFLINE } exec_type;
	char *name;
	char *filter;
	int verbose_level;
} options;

#endif