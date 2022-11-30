#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "graph.h"
#include "salesman_thr.h"
#include "mcost.h"

enum { FALSE, TRUE };

typedef struct {
	pthread_mutex_t lock;
	queue* qu;
} queue_mutex;

typedef struct {
	pthread_mutex_t lock;
	mcost *mc;
} mcost_mutex;

static int *candidates;
static int nperms;
static int *cache[2];

static long int fact(int n);
static void enqueue(queue *qu, int *task);
static int  *dequeue(queue *qu);
static void _gen_perms(graph *g, int start_v, queue *qu, int *a_perm, int k);
static int  cost_for(graph *g, int x, int y);
static mcost_mutex* init_mcost_mutex(mcost *mc);
static queue_mutex* init_queue_mutex(queue *qu);

long int
fact(int n)
{
	if (n == 0) return 1;
	return n * fact(n - 1);
}

/* Add a new task into the queue */
void
enqueue(queue *qu, int *task)
{
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
		enqueue(qu, branch);
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

int
cost_for(graph *g, int x, int y)
{
	// Did a thread cache it?
	if (cache[x][y] != -1)
		return cache[x][y];

	edgenode *enode = g->edges[x];
	while (enode->y != y) enode = enode->next;

	if (enode != NULL)
		cache[x][y] = enode->w;

	return cache[x][y];
}

mcost_mutex*
init_mcost_mutex(mcost *mc)
{
	mcost_mutex *mm;
	if ((mm = malloc(sizeof(mcost_mutex))) != NULL) {
		mm->mc = mc;
		if (pthread_mutex_init(&mm->lock, NULL) != 0) {
			free(mm);
			return NULL;
		}
	}
	return mm;
}

queue_mutex*
init_queue_mutex(queue *qu)
{
	queue_mutex *qm;
	if ((qm = malloc(sizeof(queue_mutex))) != NULL) {
		qm->qu = qu;
		if (pthread_mutex_init(&qm->lock, NULL) != 0) {
			free(qm);
			return NULL;
		}
	}
	return qm;
}

int *
read_queue(queue_mutex *qm)
{
	int *task;
	pthread_mutex_lock(&qm->lock);
	task = dequeue(qm->qu);
	pthread_mutex_unlock(&qm->lock);

	return task;

}

int *
set_mcost(mcost_mutex *mm, int new_cost, int *path)
{
	pthread_mutex_lock(&mm->lock);
	if (mm->mc->min_cost > new_cost) {
		mm->mc->min_cost = new_cost;
		mm->mc->path     = path;
	}
	pthread_mutex_unlock(&mm->lock);
}

mcost*
tsp_threaded(graph *g, queue *qu, int start_v, int nthreads)
{
	int i;
	size_t st = sizeof(int) * (g->nvertices + 1);
	mcost *mc;

	// Initialize the cache
	for (i = 0; i < 2; i++) {
		cache[i] = malloc(st);
		memset(cache[i], 0, st);
	}

	mcost_mutex *mt = init_mcost_mutex(mc);
	queue_mutex *qm = init_queue_mutex(qu);

}
