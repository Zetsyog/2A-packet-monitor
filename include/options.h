#ifndef OPTIONS_H
#define OPTIONS_H

struct options {
	enum EXEC_TYPE { NONE, ONLINE, OFFLINE } exec_type;
	char *name;
	char *filter;
	enum VERBOSE_LEVEL { CONCISE = 1, SYNTH = 2, COMPLETE = 3 } verbose_level;
} options;

#endif