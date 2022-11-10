#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "salesman_seq.h"

int
main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "usage: salesman_seq <starting_vertex> <graph_file>\n");
		exit(1);
	}

	int start_v = strtol(argv[1], NULL, 10);
	FILE *f     = fopen(argv[2], "r");
	if (f == NULL) {
		fprintf(stderr, "%s: couldn't open file '%s'\n", argv[0], argv[2]);
		exit(1);
	}

	graph g;
	g.nedges = 0;
	if (!read_graph(&g, f)) {
		fprintf(stderr, "%s: couldn't read the graph\n", argv[2]);
		exit(1);
	}
	if ( start_v >= g.nvertices || start_v <= 0 ) {
		fprintf(stderr, "%s: vertex '%d' isn't correct\n", argv[0], argv[1]);
		exit(1);
	}

	visited = malloc(sizeof(int) * g.nvertices);
	display_graph(&g);
	tsq_sequential(&g, start_v, start_v, 0, 0);

	fprintf(stdout, "Min path: %d\n", result_path);
	return 0;
}

void
tsq_sequential(graph *g, int start_v, int v, int min_path, int hop_count)
{
	int y;
	edgenode *enode = NULL;

	// starting vertex should be marked visited
	visited[v] = 1;

	enode = g->edges[v];
	while ( enode != NULL ) {
		y = enode->y;

		if ( !visited[y] ) {
			if ( (min_path + enode->w) < result_path || result_path == -1 ) {
				tsq_sequential(g, start_v, y, (min_path + enode->w), hop_count + 1);
				visited[y] = 0;
			}
		} else if ( y == start_v && hop_count == (g->nvertices - 1) ) {
			if ( (min_path + enode->w) < result_path || result_path == -1)
				result_path = min_path;
		}

		enode = enode->next;
	}
}
