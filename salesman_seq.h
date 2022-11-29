#ifndef _SALESMAN_SEQ_
#define _SALESMAN_SEQ_

typedef struct {
	int min_cost;
	int *path;
} mcost;

mcost *tsp_sequential(graph *g, int start_v);
void  tsp_result(mcost *mc, int svertex, int size);

#endif
