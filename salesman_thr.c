#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "graph.h"
#include "salesman_thr.h"

static int *visited;
static int result_cost;

static queue qu;
static stack sk;

// Prepare our shared variable for sync with the mutex locker
static shared_var *
init_shared_var(int *ref)
{
	shared_var *sv;
	if ((sv = malloc(sizeof(shared_var))) != NULL) {
		sv->aint = ref;
		if (pthread_mutex_init(&sv->mutex_locker, NULL) != 0) {
			free(sv);
			return NULL;
		}
	}

	return sv;
}

static void
write_in_peace(shared_var *sv, int new_value)
{
	pthread_mutex_lock(&sv->mutex_locker);
	*(sv->aint) = new_value;
	pthread_mutex_unlock(&sv->mutex_locker);
}

static int
read_in_peace(shared_var *sv)
{
	return *(sv->aint);
}

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
	qu->queue[++qu->size] = sk;
}

static stack*
dequeue(queue *qu)
{
	return qu->size != -1 ? &qu->queue[qu->size--] : NULL;
}

static void
_gen_tasks(graph *g, int start_v, int v, int hop_count)
{
	int y, w;

	visited[v - 1]  = 1;
	edgenode *enode = g->edges[v];

	while (enode != NULL) {
		y = enode->y;
		w = enode->w;

		if (!visited[y - 1]) {

			sk.arr[++sk.size][0] = y;
			sk.arr[sk.size][1] = w;
			//fprintf(stdout, "-%d->%d ", w, y);
			_gen_tasks(g, start_v, y, (hop_count + 1));
			// Faking pop(stack)
			sk.size--;
			visited[y - 1] = 0;

		} else if (hop_count == (g->nvertices - 1) && y == start_v) {

			//fprintf(stdout, "-%d->%d\n", w, y);
			sk.arr[++sk.size][0] = y;
			sk.arr[sk.size][1] = w;
			enqueue(&qu, sk);
			sk.size--;
		}

		enode = enode->next;
	}
}

queue *
gen_tasks(graph *g, int start_v)
{
	//fprintf(stdout, "---> %d <---\n", fact(3));
	visited  = malloc(g->nvertices * sizeof(int));
	for (int i = 0; i < g->nvertices; i++) visited[i] = 0;

	qu.queue = malloc(fact(g->nvertices - 1) * sizeof(stack));
	sk.size  = -1;
	qu.size  = -1;

	_gen_tasks(g, start_v, start_v, 0);

	free(visited);
	return &qu;
}

/* Pretty print of possible permutations(in other words, list the tasks) */
void
display_tasks(queue *qu)
{
	int i;
	for (i = 0; i <= qu->size; i++) {
		int j;
		for (j = 0; j <= qu->queue[i].size; j++)
			fprintf(stdout, " --%d--> %d", qu->queue[i].arr[j][1], qu->queue[i].arr[j][0]);
		fprintf(stdout, "\n");
	}
}

int
tsp_threaded(queue *qu, int nthreads) {

	int ntask = qu->size + 1;
	if (nthreads > ntask) {
		fprintf(stdout, "pthread: error: max number of threads should the ntasks");
		exit(1);
	}
	shared_var *rc     = init_shared_var(&result_cost);
	shared_var *ntasks = init_shared_var(&qu->size);

	// Because we won't modify its value, so we are sure about consistency
	
	result_cost = read_in_peace(rc);
	free(rc);
	free(ntasks);
	return result_cost;
}

/*
void *
athread(void *arg)
{
	int new_cost, queue_size;
	shared_var *sl = (shared_var *)arg;
	shared_var 

	while ((queue_size = read_in_peace(ntasks)) > 0) {
		int i = 0;
		write_in_peace(ntasks, --queue_size);
		stack sk = qu.queue[queue_size];
		for (i = 0; i <= qu.queue[queue_size].size; i++)
			new_cost += qu.queue[queue_size].arr[i][1];
		
		if (read_in_peace(rc) < new_cost) 
			write_in_peace(rc, new_cost);
	}
}
*/
