#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "graph.h"
#include "salesman_seq.h"
#include "salesman_thr.h"

#define VERSION 0.01

/* Prototypes */
void usage(const char *cmd);
graph **setup_graph(const char *gf, long int nv);

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
	char *graph_file;

	/* Magically parse command line arguments and set things up
	 * `PROGNAME' is going to be automatically set via "arg.h" 
	 */
	char *PROGNAME;
	ARG {
		case 'f':
			graph_file = GET(usage(PROGNAME));
			break;
		case 'g':
			nv = strtol(GET(usage(PROGNAME)), NULL, 10);
			break;
		case 's':
			sv = strtol(GET(usage(PROGNAME)), NULL, 10);
			break;
		case 'v':
			fprintf(stdout, "salesman-%.2f, @uy1 licensed under GPL.\n", VERSION);
			exit(0);
		default:
			usage(PROGNAME);
	} GRA;

	graph **g = setup_graph(graph, nv);

	/* TSP sequential & threaded */

	return 0;
}
