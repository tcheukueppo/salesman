#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "graph.h"
#include "salesman_seq.h"
#include "salesman_thr.h"

#define VERSION 0.01

/* Prototypes */
void usage(const char *cmd);

void
usage(const char *cmd)
{
	fprintf(stderr, "usage: %s [ -f GRAPH_FILE |"
	    " -g NUM_VERTICES ] [ -s START_VERTEX ] [ -v ]\n", cmd);
	exit(1);
}

int
main(int argc, char **argv)
{

	long int  sv = -1, nv = -1;
	char *graph_file = NULL;

	/* Going to be automatically set via "arg.h" */
	char *PROGNAME;

	ARG {
		case 'f':
			graph_file = GET(usage(PROGNAME));
			fprintf(stdout, "file: %s\n", graph_file);
			break;
		case 'g':
			nv = strtol(GET(usage(PROGNAME)), NULL, 10);
			fprintf(stdout, "nv: %d\n", nv);
			break;
		case 's':
			sv = strtol(GET(usage(PROGNAME)), NULL, 10);
			fprintf(stdout, "sv: %d\n", sv);
			break;
		case 'v':
			fprintf(stdout, "salesman-%.2f, @uy1 licensed under GPL.\n", VERSION);
			exit(0);
		default:
			usage(PROGNAME);
	} GRA;

	return 0;
}
