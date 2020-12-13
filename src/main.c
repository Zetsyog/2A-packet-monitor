#include "capture/common.h"
#include "capture/offline.h"
#include "capture/online.h"
#include "options.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USAGE "Usage : %s (-i <interface>| -o <file>) [-f <filter>] [-v <1..3>]"

int main(int argc, char **argv) {
	int opt;
	options.exec_type = NONE;
	options.verbose_level = COMPLETE;
	options.filter = NULL;

	while ((opt = getopt(argc, argv, "i:o:f:v:")) != -1) {
		switch (opt) {
		case 'i':
			if(options.exec_type != NONE) {
				log_error(USAGE, argv[0]);
			}
			options.exec_type = ONLINE;
			options.name = optarg;
			break;
		case 'o':
			if(options.exec_type != NONE) {
				log_error(USAGE, argv[0]);
			}
			options.exec_type = OFFLINE;
			options.name = optarg;
			break;
		case 'f':
			options.filter = optarg;
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

	set_max_verbosity(options.verbose_level);

	if (options.exec_type == ONLINE) {
		run_online();
	} else if (options.exec_type == OFFLINE) {
		run_offline();
	} else {
		log_error("Usage: you must specify either -i or -o option");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}