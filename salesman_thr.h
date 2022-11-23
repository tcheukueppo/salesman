#ifndef _SALESMAN_THR_
#define _SALESMAN_THR_

#define STACK_SIZE 256

typedef struct {
	int size;
	int arr[STACK_SIZE][2];
} stack;

// Queue of tasks
typedef struct {
	int size;
	stack *queue;
} queue;

queue *gen_tasks(graph *g, int start_v, int nv);
void  tsp_threaded(queue *qu);
void  display_tasks(queue *qu);

#endif
