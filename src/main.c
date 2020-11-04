#include <options.h>
#include <stdio.h>
#include <stdlib.h>
#include <capture/online.h>
#include <unistd.h>
#include "util.h"

#define USAGE "Usage : %s [-i interface] [-o file] [-f filter] [-v <1..3>]"

int main(int argc, char **argv) {
	int opt;
	options.exec_type = NONE;
	options.verbose_level = CONCISE;

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
				raler("Usage: verbose must be in range [1 - 3]");
			}
			break;
		default:
			raler(USAGE, argv[0]);
		}
	}

	if (optind != argc) {
		raler("%i %i", optind, argc);
	}

	if (options.exec_type == ONLINE) {
		run_online();
	} else if (options.exec_type == OFFLINE) {
		raler("not implemented yet");
	} else {
		raler("Usage: you must specify either -i or -o option");
	}

	exit(EXIT_SUCCESS);
}