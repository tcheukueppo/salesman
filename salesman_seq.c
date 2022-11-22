#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "salesman_seq.h"

static int *visited;
static int result_cost = -1;

static void
depth_first_search(graph *g, int start_v, int v, int cur_cost, int hop_count)
{
	int y;
	edgenode *enode = g->edges[v];

	visited[v] = 1;
	while (enode != NULL) {
		y = enode->y;

		if (!visited[y]) {
			int new_cost;

			if ( new_cost = (cur_cost + enode->w) < result_cost 
			||  result_cost == -1) {
				tsq_sequential(g, start_v, y, new_cost, hop_count + 1);
				visited[y] = 0;
			}
		} else if (y == start_v && hop_count == (g->nvertices - 1)) {

			if ((cur_cost + enode->w) < result_cost
			||  result_cost == -1) {
				// We just found another better `result_cost'
				result_cost = cur_cost + enode->w;
			}
		}
		enode = enode->next;
	}
}

int
tsp_sequential(graph *g, int start_v)
{
	visited = malloc(sizeof(int) * g->nvertices);
	depth_first_search(g, start_v, start_v, 0, 0)

	return result_cost;
}
