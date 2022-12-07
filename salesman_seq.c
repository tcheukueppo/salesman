#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "salesman_seq.h"
#include "mcost.h"
#include "common.h"

char *STSP_ERR;

static int *visited    = NULL;
static int *node_stack = NULL;

static void
depth_search(Graph *g, int start_v, int v, int cur_cost, int hop_count, Mcost *mc)
{
	int y;
	Edgenode *enode = g->edges[v];

	visited[v] = 1;
	while (enode != NULL) {
		y = enode->y;

		if (!visited[y]) {
			int new_cost;

			if ((new_cost = (cur_cost + enode->w)) < mc->min_cost || mc->min_cost == -1) {
				node_stack[hop_count] = y;
				depth_search(g, start_v, y, new_cost, hop_count + 1, mc);
				visited[y] = 0;
			}
		} else if (y == start_v && hop_count == (g->nvertices - 1)) {
			/* Do we just found another better `mc->min_cost'? */
			if ((cur_cost + enode->w) < mc->min_cost || mc->min_cost == -1) {
				node_stack[hop_count] = y;
				mc->min_cost = cur_cost + enode->w;
				memcpy(mc->path, node_stack, sizeof(int) * g->nvertices);
			}
		}

		enode = enode->next;
	}
}

Mcost *
tsp_sequential(Graph *g, int start_v)
{
	int i;
	Mcost *mc = NULL;
	size_t size = sizeof(int) * g->nvertices;

	/* Well, I think the use of `malloc' is abused, right? */
	if (((mc        = malloc(sizeof(Mcost))) 
	&&  (mc->path   = malloc(size))
	&&  (visited    = malloc(size))
	&&  (node_stack = malloc(size)) ))
	{
		mc->min_cost = -1;
		memset(visited, 0, (g->nvertices + 1) * sizeof(int));
		depth_search(g, start_v, start_v, 0, 0, mc);
	} else {
		malloc_err(STSP_ERR, "tsp_sequential()");
		if (mc)
			free(mc->path);
		free(mc);
	}

	free(visited);
	//free(node_stack);
	return mc;
}
