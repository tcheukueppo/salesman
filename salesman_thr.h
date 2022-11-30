#ifndef _SALESMAN_THR_
#define _SALESMAN_THR_

#include "mcost.h"

// Queue of tasks
typedef struct {
	long int size;
	int **q;
} queue;

queue *gen_tasks(graph *g, int start_v);
mcost *tsp_threaded(graph *g, queue *qu, int start_v, int nthreads);
void   display_queue(queue *qu, int nvertices);

#endif
