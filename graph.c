#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

static edgenode *
_find_reflect(graph *g, int i, int v)
{
	edgenode *enode = g->edges[i];

	while ( enode != NULL ) {
		if (enode->y == v) break;
		enode = enode->next;	
	}

	return enode;
}

static void
_insert_edge(graph *g, int x, int y, int w)
{
	edgenode *enode = malloc(sizeof(edgenode));

	enode->w    = w;
	enode->y    = y;
	enode->next = g->edges[x];
	g->edges[x] = enode;
}

graph *
gen_graph(int nv)
{
	int i, w, v = 1;
	unsigned long seed = 1;

	graph    *g;
	edgenode *enode = NULL;

	g->nvertices = nv++;
	while (v != nv) {
		// Generating edges for vertex `v'
		for (i = 1; i <= nv; i++) {
			if (i == v) continue;

			enode = _find_reflect(g, i, v);
			if (enode) {
				w = enode->w;
			} else {
				// Update seed to produce different pseudo-random series
				// each time `rand' runs.
				srand(seed++);
				w = ( (int)rand() % 99) + 1;
				g->nedges++;
			}
			_insert_edge(g, v, i, w);
		}
		v++;
	}

	return g;
}

void
display_graph(graph *g)
{
	int i;
	edgenode *enode;

	fprintf(stdout, "nvertices: %d, nedges: %d\n", g->nvertices, g->nedges);
	for (i = 1; i <= g->nvertices; i++) {
		fprintf(stdout, "%d\n", i);
		enode = g->edges[i];

		while (enode != NULL) {
			fprintf(stdout, "  %d, w = %d\n", enode->y, enode->w);
			enode = enode->next;
		}
	}
}
