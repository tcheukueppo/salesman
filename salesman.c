#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "graph.h"
#include "salesman_seq.h"
#include "salesman_thr.h"

#define VERSION 0.01

/* Prototypes */
static void usage(const char *cmd);
static void die(const char *string, const char *cmd, const char *subject);

void
usage(const char *cmd)
{
	fprintf(stderr, "usage: %s [ -f GRAPH_FILE | -g NUM_VERTICES ] "
	                "[ -s START_VERTEX ] [ -t NTHREADS ] [ -v ]\n", cmd);
	exit(1);
}

void
die(const char *string, const char *cmd, const char *subject)
{
	fprintf(stderr, string, cmd, subject);
	exit(1);
}

int
main(int argc, char **argv)
{

	char *PROGNAME;
	char *graph_file, nthreads = 2, nvtics = -1, svertex = -1;

	/* Magically parse command line arguments and set things up
	 * `PROGNAME' is going to be automatically set via "arg.h" 
	 */
	ARG {
		case 'f':
			graph_file = GET(usage(PROGNAME));
			break;
		case 'g':
			nvtics = strtol(GET(usage(PROGNAME)), NULL, 10);
			break;
		case 's':
			svertex = strtol(GET(usage(PROGNAME)), NULL, 10);
			break;
		case 't':
			nthreads = strtol(GET(usage(PROGNAME)), NULL , 10);
			break;
		case 'v':
			fprintf(stdout, "salesman-%.2f, @uy1 licensed under GPL.\n", VERSION);
			exit(0);
		default:
			usage(PROGNAME);
	} GRA;
	if (argc) usage(PROGNAME);

	graph *g = NULL;
	if (graph_file) {

		FILE *fh = fopen(graph_file, "r");

		if (fh != NULL) {
			if ((g = read_graph(fh)) == NULL) die("%s : ERROR: error while reading '%s'\n", PROGNAME, graph_file);
		} else {
			die("%s: ERROR: could not open '%s'\n", PROGNAME, graph_file);
		}

		svertex = (svertex == -1 ? 1 : g->nvertices);
	} else {
		nvtics  = (nvtics  == -1 ? 5 : nvtics);
		svertex = (svertex == -1 ? 1 : svertex);
		g = gen_graph(nvtics);
	}

	display_graph(g);
	/* TSP, sequential form */

	return 0;
}
