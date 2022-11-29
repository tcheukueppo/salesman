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

graph *
read_graph(FILE *fh)
{
	// PROTOCOL SPECIFICATION for STDIN
	// file_graph.gh
	// ---------------------
	// nb_vertices
	// vertex_x tail_y weight
	// ...      ...    ...

	int x, y, w, nvertices = 0;
	char buf[BUFSIZ];

	graph *g = malloc(sizeof(graph));

	if (fgets(buf, BUFSIZ, fh) == NULL)
		return NULL;

	g->nvertices = strtol(buf, NULL, 10);
	g->edges     = malloc(g->nvertices * sizeof(*(g->edges)));

	while (fgets(buf, BUFSIZ, fh)) {
		x = strtol(strtok(buf, " "),  NULL, 10);
		y = strtol(strtok(NULL, " "), NULL, 10);
		w = strtol(strtok(NULL, " "), NULL, 10);

		if (x < 0
		||  y < 0
		||  x > g->nvertices
		||  y > g->nvertices) return NULL;

		_insert_edge(g, x, y, w);
		_insert_edge(g, y, x, w);

		g->nedges++;
	}

	return g;
}

graph *
gen_graph(int nv)
{
	unsigned long seed = time(0);

	edgenode *enode = NULL;
	graph    *g     = malloc(sizeof(graph));

	g->nvertices = nv++;
	g->edges     = malloc( nv * sizeof(*(g->edges)) );

	int x, y, w = 1;
	for (x = 1; x < nv - 1; x++) {
		for (y = x + 1; y < nv; y++) {
			/* Update seed to produce different pseudo-random series
			 * each time `rand' runs.
			 */
			srand(seed++);
			w = ((int)rand() % 10) + 1;
			g->nedges++;
			_insert_edge(g, x, y, w);
			_insert_edge(g, y, x, w);
		}
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
