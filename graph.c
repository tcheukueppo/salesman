#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "graph.h"
static void
_insert_edge(graph *g, int x, int y, int w)
{
	edgenode *enode = malloc(sizeof(edgenode));

	enode->y    = y;
	enode->w    = w;
	enode->next = g->edges[x];
	g->edges[x] = enode;
}

int
read_graph(graph *g, FILE *fh)
{
	// PROTOCOL SPECIFICATION for STDIN
	// file_graph.gh
	// ---------------------
	// nb_vertices
	// vertex_x tail_y weight
	// ...      ...    ...

	int x, y, w, nvertices = 0;
	char buf[BUFSIZ];

	if (fgets(buf, BUFSIZ, fh) == NULL)
		return 1;

	g->nvertices = strtol(buf, NULL, 10);
	g->edges     = malloc(g->nvertices * sizeof(*(g->edges)));

	while (fgets(buf, BUFSIZ, fh)) {
		x = strtol(strtok(buf, " "),  NULL, 10);
		y = strtol(strtok(NULL, " "), NULL, 10);
		w = strtol(strtok(NULL, " "), NULL, 10);
		_insert_edge(g, x, y, w);
		nvertices++;
	}
	g->nedges /= 2;

	return nvertices == g->nvertices ? 0 : 1;
}

static edgenode *
_find_reflect(graph *g, int i, int v)
{
	edgenode *enode = g->edges[i];

	while (enode != NULL) {
		if (enode->y == v) break;
		enode = enode->next;	
	}

	return enode;
}


graph *
gen_graph(int nv)
{
	int i, w, v = 1;
	unsigned long seed = time(0);

	edgenode *enode = NULL;
	graph    *g     = malloc(sizeof(graph));

	g->nvertices = nv++;
	g->edges     = malloc( nv * sizeof(*(g->edges)) );
	while (v != nv) {
		// Generating edges for vertex `v'
		for (i = 1; i <= (nv - 1); i++) {
			if (i == v) continue;

			enode = _find_reflect(g, i, v);
			if (enode) {
				w = enode->w;
			} else {
				// Update seed to produce different pseudo-random series
				// each time `rand' runs.
				srand(seed++);
				w = ((int)rand() % 10) + 1;
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
