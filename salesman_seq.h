#ifndef _SALESMAN_SEQ_
#define _SALESMAN_SEQ_

#include "mcost.h"

Mcost *tsp_sequential(Graph *g, int start_v);
void  tsp_result(Mcost *mc, int svertex, int size);

#endif
