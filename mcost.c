#include <stdio.h>
#include "mcost.h"

void
print_mcost(Mcost *mc, int svertex, int size)
{
	int i;
	if (mc == NULL)
		return;

	fprintf(stdout, "m:%d,", mc->min_cost);
	for (i = 0; i < size - 1; i++)
		fprintf(stdout, "%d-", mc->path[i]);
	fprintf(stdout, "%d\n", mc->path[size - 1]);
}
