#ifndef GRAPH_H
#define GRAPH_H

/* Representation of an undirected edge (x <-> y) */
struct Edgenode {
	int y;
	int w;
	struct Edgenode *next;
};

typedef struct Edgenode Edgenode;

/* Representation of a graph */
typedef struct {
	int nedges;
	int nvertices;
	Edgenode **edges;
} Graph;


/* Function prototypes */
void   display_graph(Graph *g);
Graph *read_graph(FILE *fh);
Graph *gen_graph(int);

#endif
