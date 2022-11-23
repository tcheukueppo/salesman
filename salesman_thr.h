#ifndef _SALESMAN_THR_
#define _SALESMAN_THR_

#define STACK_SIZE 256

// Queue of tasks
typedef struct {
	int size = -1;
	int **queue;
} queue;

void tsp_threaded(graph *g);
void gen_tasks(graph *g, int nthreads);

#endif
