#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "salesman_seq.h"
#include "salesman_thr.h"

int
main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "usage: salesman <starting_vertex> <nvertices>\n");
		exit(1);
	}

	int start_v = strtol(argv[1], NULL, 10);
	int nv      = strtol(argv[2], NULL, 10);

	if ( start_v >= nv || start_v <= 0 ) {
		fprintf(stderr, "%s: vertex '%d' isn't correct\n", argv[0], argv[1]);
		exit(1);
	}

	graph *g = gen_graph(nv)
	display_graph(g);

	int min_cost = tsq_sequential(g, start_v);
	fprintf(stdout, "Sequential, Min path: %d\n", min_cost);

	//min_cost = tsq_threaded(g, start_v);
	//fprintf(stdout, "Multithreaded, Min path: %d\n", min_cost);

	return 0;
}
