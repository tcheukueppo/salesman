#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "common.h"
#include "mcost.h"

#include "graph.h"
#include "salesman_seq.h"
#include "salesman_thr.h"

#define VERSION 0.01

/* Prototypes */
static void usage(const char *cmd, int exit_code);
static void die(const char *string, const char *cmd, const char *subject);

void
usage(const char *cmd, int exit_code)
{
	fprintf(stderr, "usage: %s [ -f GRAPH_FILE | -g NUM_VERTICES ] "
	                "[ -s START_VERTEX ] [ -t NTHREADS ] [ -v ]\n", cmd);
	exit(exit_code);
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
	char *graph_file;
	int  nthreads = -1, nvtics = -1, svertex = -1;

	ARG {
		case 'f':
			graph_file = GET(usage(PROGNAME, 1));
			break;
		case 'g':
			nvtics = strtol(GET(usage(PROGNAME, 1)), NULL, 10);
			break;
		case 's':
			svertex = strtol(GET(usage(PROGNAME, 1)), NULL, 10);
			break;
		case 't':
			nthreads = strtol(GET(usage(PROGNAME, 1)), NULL , 10);
			break;
		case 'v':
			fprintf(stdout, "salesman-%.2f, @uy1 licensed under GPL.\n", VERSION);
			exit(0);
		case 'h':
			usage(PROGNAME, 0);
		default:
			usage(PROGNAME, 1);
	} GRA;
	if (argc) usage(PROGNAME, 1);

	Graph *g;
	if (graph_file) {

		FILE *fh = fopen(graph_file, "r");
		if (fh == NULL)
			die("%s: ERROR: could not open '%s'\n", PROGNAME, graph_file);

		if ((g = read_graph(fh)) == NULL)
			die("%s : ERROR: error while reading '%s'\n", PROGNAME, graph_file);
		svertex = (svertex == -1 ? g->nvertices : svertex);
	} else {
		nvtics  = (nvtics  == -1 ? 5 : nvtics);
		svertex = (svertex == -1 ? 1 : svertex);
		g       = gen_graph(nvtics);
	}

	nthreads = (nthreads == -1 ? 1 : nthreads);
	display_graph(g);

	Mcost *mc;
	if ( (mc = tsp_sequential(g, svertex)) == NULL ) {
		fprintf(stderr, "error: tsp_sequential()\n");
		exit(1);
	}

	print_mcost(mc, svertex, g->nvertices);

	Queue *qu = gen_tasks(g, svertex);
	display_queue(qu, g->nvertices - 2);

	printf("works?\n");
	Mcost *mc2 = tsp_threaded(g, qu, svertex, nthreads);
	print_mcost(mc2, svertex, g->nvertices);

	/* TSP, sequential form */

	free_queue(qu);
	free(mc->path);
	free(mc);
	free(mc2->path);
	free(mc2);

	return 0;
}
