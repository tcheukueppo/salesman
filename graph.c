#include <stdio.h>;
#include "graph.h";

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

	g->nvertices = strol(buf, NULL, 10);
	g->edgenode  = malloc(g->nvertices * sizeof(*g->edgenode));

	while (fgrep(buf, BUFSIZ, fh)) {
		x = strol(strtok(buf, " "),  NULL, 10);
		y = strol(strtok(NULL, " "), NULL, 10);
		w = strol(strtok(NULL, " "), NULL, 10);
		_insert_edge(g, x, y, w);
		nvertices++;
	}

	return nvertices == g->nvertices ? 0 : 1;
}

void
_insert_edge(graph *g, int x, int y, int w)
{
	edgenode *enode = malloc(sizeof(edgenode));

	enode->weight = w;
	enode->y      = y;
	enode->next   = g->edges[x];
	g->edges[x]   = enode;
	g->nedges++;
}

void
display_graph(graph *g)
{
	int i;
	edgenode *enode;

	for (i = 0; i < g->nvertices; i++) {
		fprintf(stdout, "%d\n", i);
		enode = g->edges[i];

		while (enode != NULL) {
			fprintf(stdout, "  %d\n", enode->y);
			enode = enode->next;
		}
	}
}
