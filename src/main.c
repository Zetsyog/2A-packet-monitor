#include <options.h>
#include <stdio.h>
#include <stdlib.h>
#include <capture/online.h>
#include <unistd.h>
#include "util.h"

#define USAGE "Usage : %s (-i <interface>| -o <file>) [-f <filter>] [-v <1..3>]"

int main(int argc, char **argv) {
	int opt;
	options.exec_type = NONE;
	options.verbose_level = COMPLETE;

	while ((opt = getopt(argc, argv, "i:o:f:v:")) != -1) {
		switch (opt) {
		case 'i':
			options.exec_type = ONLINE;
			options.name = optarg;
			break;
		case 'o':
			options.exec_type = OFFLINE;
			options.name = optarg;
			break;
		case 'f':
			// TODO : filter
			printf("WARNING : filter not implemented. Ignoring arg\n");
			break;
		case 'v':
			options.verbose_level = atoi(optarg);
			if (options.verbose_level < 1 || options.verbose_level > 3) {
				log_error("usage: verbose must be in range [1 - 3]");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			log_error(USAGE, argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind != argc) {
		log_error("%i %i", optind, argc);
		exit(EXIT_FAILURE);
	}

	set_verbosity(options.verbose_level);

	if (options.exec_type == ONLINE) {
		run_online();
	} else if (options.exec_type == OFFLINE) {
		log_error("not implemented yet");
		exit(EXIT_FAILURE);
	} else {
		log_error("Usage: you must specify either -i or -o option");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}