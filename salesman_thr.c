#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


int
main(int argc, char **argv)
{
	if (argc != 4) {
		fprintf(stderr, "usage: salesman_seq <starting_vertex> <nthreads> <graph_file>\n");
		exit(1);
	}

	start_v  = strtol(argv[1], NULL, 10);
	nthreads = strtol(argv[1], NULL, 10);

	FILE *f = fopen(argv[2], "r");
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
	gen_tasks(graph *g, int v)
	tsq_threads(&g);

	return 0;
}

void
gen_tasks(graph *g, int v, int *iqueue, int hop_count)
{
	int y;
	edgenode *enode = NULL;

	// Starting vertex should be marked visited
	visited[v] = 1;

	enode = g->edges[v];
	while ( enode != NULL ) {
		y = enode->y;

		if ( !visited[y] ) {
			sk.stack[++sk.size] = y;
			visited[y]          = 0;
		} else if ( y == start_v && hop_count == (g->nvertices - 1) ) {
			sk.stack[++sk.size] = y;
			enqueue(sk.stack, sk.size);
		}

		enode = enode->next;
	}

	sk.size--;
}

int *
dequeue(queue qu)
{
	return qu.queue[qu.size--];
}

int
enqueue(queue qu, )
{
	
	return 1;
}

void
tsq_threads(graph *g)
{
}
