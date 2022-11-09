#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

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

void
_insert_edge(graph *g, int x, int y, int w)
{
	edgenode *enode = malloc(sizeof(edgenode));

	enode->w    = w;
	enode->y    = y;
	enode->next = g->edges[x];
	g->edges[x] = enode;
	g->nedges++;
}

void
display_graph(graph *g)
{
	int i;
	edgenode *enode;

	fprintf(stdout, "nvertices: %d, nedges: %d\n", g->nvertices, g->nedges);
	for (i = 0; i <= g->nvertices; i++) {
		fprintf(stdout, "%d\n", i);
		enode = g->edges[i];

		while (enode != NULL) {
			fprintf(stdout, "  %d, w = %d\n", enode->y, enode->w);
			enode = enode->next;
		}
	}
}
