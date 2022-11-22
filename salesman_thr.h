#ifndef _SALESMAN_THR_
#define _SALESMAN_THR_

#define STACK_SIZE 256

struct stack {
	int size = -1;
	int stack[STACK_SIZE];
};

typedef struct stack stack;
stack sk;

struct queue {
	int size = -1;
	int **queue;
};

typedef struct queue queue;
queue qu;

int nthreads;
int *costs;

int *visited;
int start_v;

// -- Prototypes
// Generate tasks
void dequeue(queue);
void enqueue(queue, int *, int);
void gen_tasks(graph *, int);
void tsp_threads(graph *);

#endif
