#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

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

extern char *TTSP_ERR, *STSP_ERR;

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

	char *PROGNAME = NULL;
	char *graph_file = NULL;
	int  nthreads = -1, nvtics = 5, svertex = -1, debug = 0;

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
		case 'd':
			debug = strtol(GET(usage(PROGNAME, 1)), NULL , 10);
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

	Graph *g = NULL;
	if (graph_file != NULL) {

		FILE *fh = fopen(graph_file, "r");
		if (fh == NULL)
			die("%s: ERROR: could not open '%s'\n", PROGNAME, graph_file);

		if ((g = read_graph(fh)) == NULL)
			die("%s : ERROR: error while reading '%s'\n", PROGNAME, graph_file);
		svertex = (svertex == -1 ? g->nvertices : svertex);
	} else {
	fprintf(stdout, "check\n");
		nvtics = (nvtics  == -1 ? 5 : nvtics);
		g = gen_graph(nvtics);
	}

	//display_graph(g);

	/* Benchmarking */
	long int start, end;
	struct timeval *tv = malloc(sizeof(struct timeval));

	if (!gettimeofday(tv, NULL)) {
		Mcost *mc;
		start = tv->tv_sec * pow(10, 9) + tv->tv_usec;

		if ( (mc = tsp_sequential(g, svertex)) == NULL ) {
			fprintf(stderr, "%s: %s\n", PROGNAME, STSP_ERR);
			exit(1);
		}

		if (!gettimeofday(tv, NULL)) {
			end = tv->tv_sec * pow(10, 9) + tv->tv_usec;
			print_mcost(mc, svertex, g->nvertices);
			fprintf(stdout, "time: %ld\n", end - start);
		} else {
			fprintf(stderr, "error: gettimeofday()\n");
		}

		if (mc)
			free(mc->path);
		free(mc);
	} else {
		fprintf(stderr, "error: gettimeofday()\n");
	}

	Queue *qu;
	if ( (qu = gen_tasks(g, svertex)) == NULL ) {
		fprintf(stderr, "%s: %s\n", PROGNAME, TTSP_ERR);
		exit(1);
	}

	display_queue(qu, g->nvertices - 2);

	if (!gettimeofday(tv, NULL)) {
		start = tv->tv_sec * pow(10, 9) + tv->tv_usec;
		nthreads = (nthreads == -1 ? 1 : nthreads);
		Mcost *mc2;

		if ((mc2 = tsp_threaded(g, qu, svertex, nthreads, debug)) == NULL) {
			fprintf(stderr, "%s: %s\n", PROGNAME, TTSP_ERR);
			exit(1);
		}

		if (!gettimeofday(tv, NULL)) {
			end = tv->tv_sec * pow(10, 9) + tv->tv_usec;
			print_mcost(mc2, svertex, g->nvertices);
			fprintf(stdout, "time: %ld\n", end - start);
		} else {
			fprintf(stderr, "error: gettimeofday()\n");
		}

		if (mc2)
			free(mc2->path);
		free(mc2);
	} else {
		fprintf(stderr, "error: gettimeofday()\n");
	}

	free_queue(qu);
	free(tv);
	
	return 0;
}
