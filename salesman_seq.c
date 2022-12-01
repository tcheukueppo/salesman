#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "salesman_seq.h"
#include "mcost.h"

static int *visited;
static int *node_stack;

static void
depth_search(graph *g, int start_v, int v, int cur_cost, int hop_count, mcost *mc)
{
	int y;
	edgenode *enode = g->edges[v];

	visited[v - 1] = 1;
	while (enode != NULL) {
		y = enode->y;

		if (!visited[y - 1]) {
			int new_cost;

			if ((new_cost = (cur_cost + enode->w)) < mc->min_cost 
			||  mc->min_cost == -1) {

				node_stack[hop_count] = y;
				depth_search(g, start_v, y, new_cost, hop_count + 1, mc);
				visited[y - 1] = 0;
			}
		} else if (y == start_v && hop_count == (g->nvertices - 1)) {

			/* Do we just found another better `mc->min_cost'? */
			if ((cur_cost + enode->w) < mc->min_cost
			||  mc->min_cost == -1) {

				node_stack[hop_count] = y;
				mc->min_cost = cur_cost + enode->w;
				memcpy(mc->path, node_stack, sizeof(int) * g->nvertices);
			}
		}
		enode = enode->next;
	}
}

mcost*
tsp_sequential(graph *g, int start_v)
{
	int i;
	mcost *mc   = malloc(sizeof(mcost));
	mc->min_cost = -1;

	size_t size = sizeof(int) * g->nvertices;
	visited    = malloc(size);
	node_stack = malloc(size);
	mc->path   = malloc(size);

	/* Clear garbages */
	memset(visited, 0, (g->nvertices - 1) * sizeof(int));

	depth_search(g, start_v, start_v, 0, 0, mc);

	free(visited);
	free(node_stack);
	return mc;
}

void
tsp_result(mcost *mc, int svertex, int size)
{
	int i;

	fprintf(stdout, "min_cost: %d\n", mc->min_cost);
	fprintf(stdout, "\t%d --> ", svertex);
	for (i = 0; i < size - 1; i++)
		fprintf(stdout, "%d --> ", mc->path[i]);
	fprintf(stdout, "%d\n", mc->path[size - 1]);
}
