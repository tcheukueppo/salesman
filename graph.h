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

void _insert_edge(graph *g, int x, int y, int w);
int  read_graph(graph *g, FILE *fh);
void display_graph(graph *g);
