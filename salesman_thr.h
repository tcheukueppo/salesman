#ifndef _SALESMAN_THR_
#define _SALESMAN_THR_

#define MAX_NTHREADS 100

#include "mcost.h"

/* Queue of tasks */
typedef struct {
	long int size;
	int **q;
} Queue;

Queue *gen_tasks(Graph *g, int start_v);
Mcost *tsp_threaded(Graph *g, Queue *qu, int start_v, int nthreads, int debug);
void  display_queue(Queue *qu, int nvertices);
void  free_queue(Queue *qu);

#endif
