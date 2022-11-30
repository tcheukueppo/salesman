#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "graph.h"
#include "salesman_thr.h"
#include "mcost.h"

enum { FALSE, TRUE };

static int *candidates;
static int nperms;

static long int  fact(int n);
static void enqueue(queue *qu, int *stack, int size);
static int  *dequeue(queue *qu);
static void _gen_perms(graph *g, int start_v, queue *qu, int *a_perm, int k);

long int
fact(int n)
{
	if (n == 0) return 1;
	return n * fact(n - 1);
}

/* Add a new task to the queue */
void
enqueue(queue *qu, int *task, int size)
{
	int *mem;
	qu->q[++qu->size] = task;
}

/* Remove a task from the queue */
int *
dequeue(queue *qu)
{
	return qu->size != -1 ? qu->q[qu->size--] : NULL;
}

void
_gen_perms(graph *g, int start_v, queue *qu, int *a_perm, int k)
{
	// Check if we have exhausted the list of candidates
	if (k == (g->nvertices - 1)) {
		int *branch = malloc((sizeof(int) * k));

		memcpy(branch, a_perm, sizeof(int) * k);
		enqueue(qu, branch, k);
		return;
	}

	int i;
	for (i = 1; i <= g->nvertices; i++) {
		if (i == start_v) continue;
		if (candidates[i] == TRUE) {
			candidates[i] = FALSE;
			a_perm[k]     = i;
			_gen_perms(g, start_v, qu, a_perm, k + 1);
			candidates[i] = TRUE;
		}
	}
}

queue *
gen_tasks(graph *g, int start_v)
{
	int i;
	queue *qu   = malloc(sizeof(queue));
	candidates  = malloc(sizeof(int) * (g->nvertices + 1));
	int *a_perm = malloc(sizeof(int) * g->nvertices);

	/* All be candidates!! */
	for (i = 1; i <= g->nvertices; i++) candidates[i] = TRUE;

	/* `start_v' should never be a candidate */
	candidates[start_v] = FALSE;

	qu->size = -1;
	nperms   = fact(g->nvertices - 1);
	qu->q    = malloc(sizeof(int *) *  nperms);
	_gen_perms(g, start_v, qu, a_perm, 0);

	free(a_perm);
	free(candidates);
	return qu;
}

void
display_queue(queue *qu, int nvertices) {
	int j;
	long int i;

	for (i = 0; i <= qu->size; i++) {
		for (j = 0; j < nvertices; j++)
			fprintf(stdout, "%d --> ", qu->q[i][j]);
		fprintf(stdout, "%d\n", qu->q[i][nvertices]);
	}
}
