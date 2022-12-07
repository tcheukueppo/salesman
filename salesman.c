#include <stdio.h>
#include <time.h>
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
	                "[ -s START_VERTEX ] [ -t NTHREADS ] [ -v|-i|-h ] "
	                "[ -d DEBUG_FLAG ]\n", cmd);
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

	if (argc)
		usage(PROGNAME, 1);

	Graph *g = NULL;
	if (graph_file != NULL) {
		FILE *fh = fopen(graph_file, "r");

		if (fh == NULL)
			die("%s: ERROR: could not open '%s'\n", PROGNAME, graph_file);

		if ((g = read_graph(fh)) == NULL)
			die("%s: ERROR: error while reading '%s'\n", PROGNAME, graph_file);

		svertex = (svertex == -1 ? g->nvertices : svertex);
	} else {
		nvtics = (nvtics == -1 ? 5 : nvtics);
		svertex = (svertex == -1 ? 1 : svertex);
		g = gen_graph(nvtics);
	}

	display_graph(g);

	/* Benchmarking */
	char start[BUFSIZ], end[BUFSIZ];
	float cpu_time_used;
	struct timespec cputime;

	Mcost *mc;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cputime);
	sprintf(start, "%lld.%09ld", (long long)cputime.tv_sec, cputime.tv_nsec);
	//fprintf(stdout, "start: %s\n", start);
	if ((mc = tsp_sequential(g, svertex)) == NULL)
		die("%s: %s\n", PROGNAME, STSP_ERR);

	fprintf(stdout, "done?\n");
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cputime);
	sprintf(end, "%lld.%09ld", (long long)cputime.tv_sec, cputime.tv_nsec);
	cpu_time_used = strtof(end, NULL) - strtof(start, NULL);
	fprintf(stdout, "seq, time:%.9f\n", cpu_time_used);
	//fprintf(stdout, "end: %s\n", end);
	print_mcost(mc, svertex, g->nvertices);

	if (mc)
		free(mc->path);
	free(mc);

	Queue *qu;
	if ((qu = gen_tasks(g, svertex)) == NULL)
		die("%s: %s\n", PROGNAME, TTSP_ERR);

	//display_queue(qu, g->nvertices - 2);

	nthreads = (nthreads == -1 ? 2 : nthreads);

	Mcost *mc2;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cputime);
	sprintf(start, "%lld.%09ld", (long long)cputime.tv_sec, cputime.tv_nsec);
	//fprintf(stdout, "\nstart: %s\n", start);

	if ((mc2 = tsp_threaded(g, qu, svertex, nthreads, debug)) == NULL)
		die("%s: %s\n", PROGNAME, TTSP_ERR);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cputime);
	sprintf(end, "%lld.%09ld", (long long)cputime.tv_sec, cputime.tv_nsec);

	cpu_time_used = strtof(end, NULL) - strtof(start, NULL);
	fprintf(stdout, "threaded, time:%.9f\n", cpu_time_used);
	//fprintf(stdout, "end: %s\n", end);
	print_mcost(mc2, svertex, g->nvertices);

	if (mc2)
		free(mc2->path);
	free(mc2);

	free_queue(qu);
	return 0;
}
