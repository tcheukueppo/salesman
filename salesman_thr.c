#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "graph.h"
#include "salesman_thr.h"


static int *visited;
static int result_cost;

static queue qu;
static stack sk;

static void enqueue(queue *qu, int *stack)

static int
fact(int n)
{
	if ( n == 0 ) return 1;
	return n * fact(n - 1);
}

/* Add a new task to the queue */
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

queue *
gen_tasks(graph *g, int start_v)
{
	return &qu;
}

