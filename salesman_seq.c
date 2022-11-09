#include <stdio.h>
#include "graph.h"

static int *visited;
static int *explored;
static int result_path = -1;

int tsq_sequential(graph *, int);

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
		fprintf(stderr, "%s: couldn't open file '%s'\n", argv[0], argv[2]);
		exit(1);
	}

	graph g;
	if (!read_graph(&g, f)) {
		fprintf(stderr, "%s: couldn't read the graph\n", argv[2]);
		exit(1);
	}
	if ( start_v >= g->nvertices || start_v <= 0 ) {
		fprintf(stderr, "%s: vertex '%d' isn't correct\n", argv[0], argv[1]);
		exit(1);
	}

	explored_vertices = malloc(sizeof(int) * g->nvertices);
	visited_vertices  = malloc(sizeof(int) * g->nvertices);
	fprintf(stdout, "Min path: %d\n", tsp_sequential(g, start_v, 0, -1));
	return 0;
}

void
tsq_sequential(graph *g, int v, int min_path, int start_v)
{
	int y;
	edgenode *enode = NULL;

	// starting vertex should be marked visited but not explored
	visited[v] = 1;

	enode = g->edges[v];
	while (enode != NULL) {
		y = enode->y;

		if (!visited[y])
			if ( (min_path + enode->w) < result_path || result_path == -1 )
				tsq_sequential(g, y, (min_path + enode->w), start_v);

		else if (!explored[y])
			if (y == start_v)
				if ( (min_path + enode->w) < result_path || result_path == - 1 )
					result_path = min_path + enode->w;

		enode = enode->next;
	}

	explored[v] = 1;
}
