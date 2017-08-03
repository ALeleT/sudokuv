#ifndef GRAPH_H
#define GRAPH_H

struct edge {
	int index_of_edge;
	struct edge * next;
};

struct edge_list {
	struct edge * head;
	int num_edges;
};

struct graph {
	// TODO: add support for directed vs undirected
	// currently only support for directed graphs
	int size;
	int * values;
	struct edge_list * edge_list_array;
};

void add_node(struct graph * src, int value);
void add_edge(struct edge_list * list, int index);
void create_edge_list_array(struct edge_list ** list_array, int size);
void print_graph(struct graph * src);
void new_graph(struct graph ** dest, int size, int * value_array);
void free_graph(struct graph ** target);

// TODO: add BFS
// TODO: add DFS

#endif
