#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "graph.h"
#include "salesman_thr.h"

static int *visited;
static queue qu;

// Well, idk, fact() isn't in `math.h' where is it?
static int
fact(int n)
{
	if ( n == 0 ) return 1;
	return n * fact(n - 1);
}

static void
enqueue(queue *qu, stack sk)
{
	if ( qu->size == sizeof(qu->queue) / sizeof(stack *) ) {
		fprintf(stderr, "enqueue: error: buffer overflow\n");
		exit(1);
	}

	qu->queue[qu->size++] = sk;
}

static stack*
dequeue(queue *qu)
{
	return qu->size != 0 ? &qu->queue[qu->size--] : NULL;
}

static queue *
_gen_tasks(graph *g, int start_v, int v, int hop_count)
{
	int y, w;
	static stack sk;

	visited[v - 1]  = 1;
	edgenode *enode = g->edges[v];
	while (enode != NULL) {
		y = enode->y;
		w = enode->w;

		if (!visited[y - 1]) {

			sk.arr[++sk.size][0] = y;
			sk.arr[++sk.size][1] = w;
			_gen_tasks(g, start_v, y, ++hop_count);
			// Faking pop(stack)
			sk.size--;
			visited[y - 1] = 0;

		} else if (++hop_count == g->nvertices && y == start_v) {

			sk.arr[++sk.size][0] = y;
			sk.arr[++sk.size][1] = w;
			enqueue(&qu, sk);
		}

		enode = enode->next;
	}
}

queue *
gen_tasks(graph *g, int start_v, int nv)
{
	visited   = malloc(nv * sizeof(int));
	qu.queue = malloc(fact(nv - 1) * sizeof(stack));

	free(visited);
	return _gen_tasks(g, start_v, start_v, 0);
}

/* Pretty print of the possible permutations(in other words, list the tasks) */
void
display_tasks(queue *qu)
{
	int i;
	for (i = 0; i < qu->size; i++) fprintf(stdout, "|%-5d", 1);

	for (i = 0; i < qu->queue[0].size; i++) {
		int j;

		/* Output vertices */
		for (j = 0; j < qu->size; j++)
			fprintf(stdout, "%-6d", qu->queue[j].arr[i][0]);

		fprintf(stdout, "\n");

		/* Output weights */
		for (j = 0; j < qu->size; j++)
			fprintf(stdout, "|%-5d", qu->queue[j].arr[i][1]);
	}

	for (i = 0; i < qu->size; i++) fprintf(stdout, "|%-5d", 1);
}

void tsp_threaded(queue *) {}
