#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "graph.h"
#include "salesman_thr.h"
#include "mcost.h"
#include "common.h"

enum { FALSE = 0, TRUE };

typedef struct {
	pthread_mutex_t lock;
	Queue* qu;
} Queue_mutex;

typedef struct {
	pthread_mutex_t lock;
	Mcost *mc;
} Mcost_mutex;

typedef struct {
	Queue_mutex *qm;
	Mcost_mutex *mm;
	Graph *g;
	int start_v;
	int debug;
} Thread_arg;

char *TTSP_ERR;

static Thread_arg targ;
static pthread_t *threads;
static int *candidates;
static int nperms;
static int **cache;
static long int fact(int n);
static int enqueue(Queue *qu, int *task, int size);
static int *dequeue(Queue *qu);
static int _gen_perms(Graph *g, int start_v, Queue *qu, int *a_perm, int k);
static int cost_for(Graph *g, int x, int y);
static Mcost_mutex *init_mcost_mutex(Mcost *mc);
static Queue_mutex *init_queue_mutex(Queue *qu);
static void *worker(void *targ);
static void *set_mcost(Mcost_mutex *mm, int new_cost, int *path);
static int  read_mcost(Mcost_mutex *mm);
static void cleanup(int nvertices);

long int
fact(int n)
{
	if (!n) return 1;
	return n * fact(n - 1);
}

/* Add a new task into the queue */
int
enqueue(Queue *qu, int *task, int size)
{
	if ( (qu->q[++qu->size] = malloc(sizeof(int) * size)) == NULL )
		return 1;

	memcpy(qu->q[qu->size], task, sizeof(int) * size);
	return 0;
}

/* Remove a task from the queue */
int *
dequeue(Queue *qu)
{
	return qu->size != -1 ? qu->q[qu->size--] : NULL;
}

int
_gen_perms(Graph *g, int start_v, Queue *qu, int *a_perm, int k)
{
	/* Check if we have exhausted the list of candidates */
	if (k == (g->nvertices - 1)) {
		if (enqueue(qu, a_perm, k) == 1) {
			malloc_err(TTSP_ERR, "_gen_perms()");
			return 1;
		}
		return 0;
	}

	int i;
	for (i = 1; i <= g->nvertices; i++) {
		/* Go back to the top and report the malloc error */
		if (TTSP_ERR)
			return 1;

		if (i == start_v)
			continue;
		if (candidates[i] == TRUE) {
			candidates[i] = FALSE;
			a_perm[k]     = i;
			_gen_perms(g, start_v, qu, a_perm, k + 1);
			candidates[i] = TRUE;
		}
	}

	return TTSP_ERR != NULL ? 1 : 0;
}

Queue *
gen_tasks(Graph *g, int start_v)
{
	int i, *a_perm = NULL;
	size_t size    = sizeof(int) * g->nvertices;
	Queue *qu      = NULL;

	nperms = fact(g->nvertices - 1);

	/* tired of mallocing, I'm certain this is insane */
	if (( (qu         = malloc(sizeof(Queue)))
	&&    (candidates = malloc((g->nvertices + 1) * sizeof(int)))
	&&    (a_perm     = malloc(g->nvertices * sizeof(int)))
	&&    (qu->q      = malloc(sizeof(int *) * nperms)) )) 
	{
		/* should all be candidates!! */
		// memset(candidates, 1, (sizeof(int) * g->nvertices));
		for (int i = 0; i <= g->nvertices; i++) candidates[i] = TRUE;
		candidates[start_v] = FALSE;

		qu->size = -1;
		TTSP_ERR = NULL;
		if (_gen_perms(g, start_v, qu, a_perm, 0) != 0)
			free_queue(qu);
	} else {
		malloc_err(TTSP_ERR, "gen_tasks()");
	}

	free(a_perm);
	free(candidates);
	return qu;
}

/* Queue data structure is too complex to be freed without a wrapper */
void
free_queue(Queue *qu)
{
	if (!qu)
		return;

	if (qu->q) {
		int i;
		for (i = 0; i <= qu->size; i++)
			free(qu->q[i]);
	}

	free(qu->q);
	free(qu);
}

void
display_queue(Queue *qu, int nvertices) {
	int j;
	long int i;

	for (i = 0; i <= qu->size; i++) {
		for (j = 0; j < nvertices; j++) fprintf(stdout, "%d-", qu->q[i][j]);
		fprintf(stdout, "%d\n", qu->q[i][nvertices]);
	}
}

int
cost_for(Graph *g, int x, int y)
{
	/* Did a thread cache it? */
	if (cache[x][y] != -1)
		return cache[x][y];

	Edgenode *enode = g->edges[x];
	while (enode->y != y && enode != NULL)
		enode = enode->next;

	if (enode != NULL)
		cache[x][y] = (cache[y][x] = enode->w);

	return cache[x][y];
}

Mcost_mutex *
init_mcost_mutex(Mcost *mc)
{
	Mcost_mutex *mm = NULL;

	if ((mm = malloc(sizeof(Mcost_mutex))) != NULL ) {
		mc->min_cost = -1;
		mm->mc = mc;
		if (pthread_mutex_init(&mm->lock, NULL) != 0) {
			free(mm);
			return NULL;
		}
	}

	return mm;
}

Queue_mutex *
init_queue_mutex(Queue *qu)
{
	Queue_mutex *qm = NULL;

	if ((qm = malloc(sizeof(Queue_mutex))) != NULL) {
		qm->qu = qu;
		if (pthread_mutex_init(&qm->lock, NULL) != 0) {
			free(qm);
			return NULL;
		}
	}

	return qm;
}

int *
read_queue(Queue_mutex *qm)
{
	int *task;

	pthread_mutex_lock(&qm->lock);
	task = dequeue(qm->qu);
	pthread_mutex_unlock(&qm->lock);

	return task;
}

void *
set_mcost(Mcost_mutex *mm, int new_cost, int *path)
{
	pthread_mutex_lock(&mm->lock);
	if (mm->mc->min_cost > new_cost || mm->mc->min_cost == -1) {
		mm->mc->min_cost = new_cost;
		mm->mc->path     = path;
	}
	pthread_mutex_unlock(&mm->lock);
}

int
read_mcost(Mcost_mutex *mm)
{
	return mm->mc->min_cost;
}

void *
worker(void *targ)
{
	int *task, left = 0, valid = 0;
	Thread_arg *arg = (Thread_arg *)targ;

	while ( (task = read_queue(arg->qm)) != NULL) {
		int i = 0, cur_cost, cur_min_cost;

		cur_cost = cost_for(arg->g, arg->start_v, task[i]);
		for (i = 0; i < arg->g->nvertices - 2; i++) {

			cur_cost += cost_for(arg->g, task[i], task[i + 1]);
			cur_min_cost = read_mcost(arg->mm);
			if (cur_cost > cur_min_cost && cur_min_cost != -1) {
				left++;
				break;
			}
		}

		if (cur_cost > cur_min_cost && cur_min_cost != -1)
			continue;

		cur_cost += cost_for(arg->g, task[i], arg->start_v);
		cur_min_cost = read_mcost(arg->mm);

		if (cur_cost <= cur_min_cost || cur_min_cost == -1) {
			valid++;
			set_mcost(arg->mm, cur_cost, task);

			if (arg->debug) {
				int k;

				fprintf(stdout, "thread: %lu (cost: %d): ", pthread_self(), cur_cost);
				for (k = 0; k < arg->g->nvertices - 2; k++)
					fprintf(stdout, "%d -> ", task[k]);
				fprintf(stdout, "\n");
			}
		} else {
			left++;
		}
	}

	if (arg->debug)
		fprintf(stdout, "\nthread: %lu, validated: %d, left: %d\n", pthread_self(), valid, left);

	return (void *)0;
}

Mcost *
tsp_threaded(Graph *g, Queue *qu, int start_v, int nthreads, int debug)
{
	if (nthreads > nperms || nthreads > MAX_NTHREADS) {
		sprintf(TTSP_ERR, "tsp_threaded: ERROR: nthreads %d < 100 && %d > %d?", nthreads, nthreads, nperms);
		return NULL;
	}

	Mcost *mc;
	if (( (threads = malloc(sizeof(pthread_t) * nthreads))
	&&    (mc      = malloc(sizeof(Mcost)))
	&&    (cache   = malloc(sizeof(int *) * (g->nvertices + 1))) ))
	{
		targ.mm      = init_mcost_mutex(mc);
		targ.qm      = init_queue_mutex(qu);
		targ.g       = g;
		targ.debug   = debug;
		targ.start_v = start_v;

		int i;
		size_t st = sizeof(int) * (g->nvertices + 1);
		for (i = 1; i <= g->nvertices; i++) {
			if ((cache[i] = malloc(st)) == NULL) {
				cleanup(g->nvertices);
				malloc_err(TTSP_ERR, "cache");
				return NULL;
			}
			memset(cache[i], -1, st);
		}

		for (i = 0; i < nthreads; i++) {
			if (pthread_create(&threads[i], NULL, worker, (void *)&targ) != 0) {
				cleanup(g->nvertices);
				TTSP_ERR = "tsp_threaded: ERROR: pthread_create";
				return NULL;
			}
		}

		/* Wait for worker threads to finish their jobs */
		void *ret;
		for (i = 0; i < nthreads; i++)
			pthread_join(threads[i], &ret);

	} else {
		malloc_err(TTSP_ERR, "tsp_threaded()");
	}

	cleanup(g->nvertices);
	return mc;
}

void
cleanup(int nvertices)
{
	if (cache) {
		int i;
		for (i = 1; i <= nvertices; i++)
			free(cache[i]);
	}
	free(cache);
	free(targ.mm);
	free(targ.qm);
	free(threads);
}
