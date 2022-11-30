#ifndef _SALESMAN_THR_
#define _SALESMAN_THR_

#include "mcost.h"

// Queue of tasks
typedef struct {
	long int size;
	int **q;
} queue;

queue *gen_tasks(graph *g, int start_v);
void   display_queue(queue *qu, int nvertices);

#endif
