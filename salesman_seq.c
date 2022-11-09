#include <stdio.h>
#include "graph.h"
#include "salesman_seq.h"


void tsq_sequential(graph *, int);

int
main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "usage: salesman_seq <starting_vertex> <graph_file>\n");
		exit(1);
	}

	int start_v = strol(argv[1], NULL, 10);
	FILE *f     = fopen(argv[2], "r");
	if (f == NULL) {
		fprintf(stderr, "%s: couldn't open file '%s'\n", argv[0], argv[1]);
		exit(1);
	}

	graph g;
	if (!read_graph(&g, f)) {
		fprintf(stderr, "%s: couldn't read the graph\n", argv[2]);
		exit(1);
	}

	int min_path = tsp_sequential(&g, start_v);
	fprintf(stdout, "Min path: %d\n", tsp_sequential(g));

	return 0;
}

int
tsq_sequential(graph *g, int startv)
{
	edgenode *enode = NULL;

	
}
