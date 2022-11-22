#ifndef GRAPH_H
#define GRAPH_H
#endif /* GRAPH_H */

// Representation of an undirected edge (x - y)
struct edgenode {
	int y;
	int w;
	struct edgenode *next;
};

typedef struct edgenode edgenode;

// Representation of a graph
typedef struct {
	edgenode **edges;
	int nedges;
	int nvertices;
} graph;


// Function prototypes
void    display_graph(graph *g);
graph*  gen_graph(int);
