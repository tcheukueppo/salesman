#ifndef _MCOST_H_
#define _MCOST_H_

typedef struct {
	int min_cost;
	int *path;
} Mcost;

void print_mcost(Mcost *mc, int svertex, int size);

#endif
