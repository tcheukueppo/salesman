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

typedef struct {
	queue_mutex *qm;
	mcost_mutex *mm;
	graph       *g;
	int         start_v;
} thread_arg;

static int *candidates;
static int nperms;
static int **cache;

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
		fprintf(stdout, "%d\n|", qu->q[i][nvertices]);
	}
}

int
cost_for(graph *g, int x, int y)
{
	// Did a thread cache it?
	if (cache[x][y] != -1)
		return cache[x][y];

	edgenode *enode = g->edges[x];
	while (enode->y != y && enode != NULL) enode = enode->next;

	if (enode != NULL) {
		cache[x][y] = (cache[y][x] = enode->w);
	}

	return cache[x][y];
}

mcost_mutex*
init_mcost_mutex(mcost *mc)
{
	mcost_mutex *mm = NULL;
	if ((mm = malloc(sizeof(mcost_mutex))) != NULL) {
		mc->min_cost = -1;
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
	queue_mutex *qm = NULL;
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

void *
set_mcost(mcost_mutex *mm, int new_cost, int *path)
{
	pthread_mutex_lock(&mm->lock);
	if (mm->mc->min_cost > new_cost || mm->mc->min_cost == -1) {
		mm->mc->min_cost = new_cost;
		mm->mc->path     = path;
	}
	pthread_mutex_unlock(&mm->lock);
}

int
read_mcost(mcost_mutex *mm)
{
	return mm->mc->min_cost;
}

void *
worker(void *targ)
{
	int bads = 0, goods = 0;
	int *task, cur_cost;
	thread_arg *arg = (thread_arg *)targ;

	while ( (task = read_queue(arg->qm)) != NULL) {
		int i = 0, rm;

		cur_cost = cost_for(arg->g, arg->start_v, task[i]);
		for (i = 0; i < arg->g->nvertices - 2; i++) {
			cur_cost += cost_for(arg->g, task[i], task[i + 1]);
			rm = read_mcost(arg->mm);
			if (cur_cost > rm && rm != -1) {
				bads++;				
				break;
			}
		}
		cur_cost += cost_for(arg->g, task[i], arg->start_v);
		rm = read_mcost(arg->mm);
		if (cur_cost < rm || rm == -1) {
			set_mcost(arg->mm, cur_cost, task);
			fprintf(stdout, "thread: %d(cost: %d): ", pthread_self(), cur_cost);
			for (int k = 0; k < arg->g->nvertices - 2; k++)
				fprintf(stdout, "%d -> ", task[k]);
			fprintf(stdout, "\n");

			goods++;
		}
	}

	fprintf(stdout, "\nThread: %d, goods: %d, bads: %d\n", pthread_self(), goods, bads);

	return (void *)0;
}

mcost*
tsp_threaded(graph *g, queue *qu, int start_v, int nthreads)
{
	if (nthreads > nperms || nthreads > 100) {
		fprintf(stderr, "tsp_threaded: ERROR: nthreads: '%d' isn't proper\n", nthreads);
		exit(1);
	}

	pthread_t *threads = malloc(sizeof(pthread_t) * nthreads);
	mcost *mc = malloc(sizeof(mcost));
	thread_arg targ;
	targ.mm = init_mcost_mutex(mc);
	targ.qm = init_queue_mutex(qu);
	targ.g = g;
	targ.start_v = start_v;

	/* Initialize cache to speed up the `cost' lookup */

	cache = malloc(sizeof(int *) * (g->nvertices + 1));

	int i;
	size_t st = sizeof(int) * (g->nvertices + 1);
	for (i = 1; i <= g->nvertices; i++) {
		cache[i] = malloc(st);
		memset(cache[i], -1, st);
	}

	for (i = 0; i < nthreads; i++) {
		if (pthread_create(&threads[i], NULL, worker, (void *)&targ) != 0) {
			fputs("tsp_threaded: ERROR: pthread_create: could initiate a thread, exit program with failure", stderr);
			free(threads);
			exit(1);
		}
	}

	/* Wait for worker threads to finish their jobs */
	void *ret;
	for (i = 0; i < nthreads; i++)
		pthread_join(threads[i], &ret);

	for (i = 1; i <= g->nvertices; i++) free(cache[i]);
	free(cache);
	free(threads);
	return mc;
}
