#ifndef _SALESMAN_SEQ_
#define _SALESMAN_SEQ_

#include "mcost.h"

mcost *tsp_sequential(graph *g, int start_v);
void  tsp_result(mcost *mc, int svertex, int size);

#endif
