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

typedef struct {
	int *aint;
	pthread_mutex_t mutex_locker;
} shared_var;

queue *gen_tasks(graph *g, int start_v);
int   tsp_threaded(queue *qu, int nthreads);
void  display_tasks(queue *qu);

#endif
