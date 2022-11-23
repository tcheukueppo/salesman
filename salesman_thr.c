#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "graph.h"
#include "salesman_thr.h"

#define STACK_SIZE 256

typedef struct {
	int size;
	int arr[STACK_SIZE][2];
} stack;

static int result_cost;
static int *visited;
static queue qu;

// -- Prototypes
// Generate tasks
static void dequeue(queue);
static void enqueue(queue *, int **, int);
static void gen_tasks(graph *, int);

struct TODO {
	int result_cost;
	pthread_mutex_t cost_locker;
};

// Well, idk, fact() isn't in math.h where is it?
static int
fact(int n)
{
	if ( n == 0 ) return 1;
	return n * fact(n - 1);
}

static void
enqueue(queue *qu, stack sk)
{
	qu->queue[qu->size++] = sk;
}

static int **
dequeue(queue *qu)
{
	return qu->size != 0 ? qu->queue[qu->size--] : NULL;
}

static queue *
_gen_tasks(graph *g, int start_v, int v, int hop_count)
{
	static stack sk;

	visited[v - 1] = 1;
	egdenode *enode = g->edges[v];
	while (enode != NULL) {
		y = enode->y;

		if (!visited[y - 1]) {

			sk.arr[++sk.size][0] = y;
			sk.arr[++sk.size][1] = w;
			_gen_tasks(g, start_v, y, ++hop_count);
			visited[y - 1] = 0;
			sk.size--;

		} else if (++hop_count == g->nvertices && y == start_v) {

			sk.arr[++sk.size][0] = y;
			sk.arr[++sk.size][1] = w;
			enqueue(qu, sk);
		}

		enode = enode->next;
	}
}

queue *
gen_tasks(graph *g, int start_v, int nv)
{
	visited   = malloc(nv * sizeof(int));
	qu->queue = malloc(fact(nv - 1) * sizeof(int *));
	return _gen_tasks(g, start_v);
}

/* Pretty print of the possible permutations(in other words, list the tasks) */
void
display_queue(queue *qu)
{
	int i;
	for (i = 0; i < qu->size; i++) fprintf(stdout, "%-5d ", 1); 

	for (i = 0; i < qu->queue[0].size; i++) {
		int j;

		/* Output vertices */
		for (j = 0; j < qu->size; j++)
			fprintf(stdout, "%-5d ", qu->queue[j].stack[i][0]);

		fprintf(stdout, "\n");

		/* Output weights */
		for (j = 0; j < qu->size; j++)
			fprintf(stdout, "|%-4d", qu->queue[j].stack[i][1]);
	}

	for (i = 0; i < qu->size; i++) fprintf(stdout, "%-5d ", 1); 
}

void
tsp_threaded(graph *, int nv)
{
	visited = {0};
	free(visited);
	return;
}
