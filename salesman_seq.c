#include <stdio.h>
#include "graph.h"
#include "salesman_seq.h"

static int *visited_vertices;
static int *explored_vertices;
static int found_min_path = 0;

static void tsq_sequential(graph *, int);

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

	explored_vertices = malloc(sizeof(int) * g->nvertices);
	visited_vertices  = malloc(sizeof(int) * g->nvertices);
	fprintf(stdout, "Min path: %d\n", tsp_sequential(g, start_v, found_min_path));
	return 0;
}

int
tsq_sequential(graph *g, int v, int min_path, int result_path)
{
	edgenode *enode = NULL;

	// starting vertex should be marked visited but not explored
	visited_vertices[v] = 1;

	enode = g->edges[v];
	while (enode != NULL) {
		y = enode->y;

		if (!visited[y]) {
			min_path += enode->w;
			if ( min_path < result_path || result_path == -1 ) {
				tsq_sequential(g, y, min_path, result_path);
			} else {
				// fprintf(stdout, "giving up at %d\n", v);
				return 0;
			}
		}
		else if (!discovered[y]) {
			if (y == start_v) {
				min_path += enode->w;
				if ( min_path < result_path || result_path == - 1 )
					result_path = min_path;
			}
		}
		enode = enode->next;
	}

	discovered[v] = 1;

	return result_path;
}
