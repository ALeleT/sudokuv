#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
//TODO: error handling ALL

void free_graph(struct graph ** target){
	free((*target)->values);

	// free each node inside each edge_list
	for(int i = 0; i < (*target)->size; i++){
		struct edge * cur = (*target)->edge_list_array[i].head;
		struct edge * prev = cur;
		while(cur != NULL && cur->next != NULL){
			prev = cur;
			cur = cur->next;
			free(prev);
		} if(cur) free(cur);
	}

	// free edge_list_array, and then free graph struct
	free((*target)->edge_list_array);
	free((*target));
}

// size is the number of initial nodes
// value_array is a pointer to an array of size that populates in order node values
// if value_array is NULL, or size is 0, or both, edge_list_array will be NULL
// TODO: add support for array of linked lists and size to instantiate graph with adjacency list
void new_graph(struct graph ** dest, int size, int * value_array){
	(*dest) = malloc(sizeof(struct graph));
	(*dest)->size = size;
	
	// default values pointer to NULL
	// and
	// default edge_list_array to NULL
	(*dest)->values = NULL;
	(*dest)->edge_list_array = NULL;

	// check for NULL array
	if(value_array == NULL) {
		(*dest)->size = 0;
	}

	// handle size / value_array here
	if((*dest)->size != 0){
		(*dest)->values = malloc(sizeof(int) * (*dest)->size);
		// copy value array into values
		for(int i = 0; i< (*dest)->size; i++){
			(*dest)->values[i] = value_array[i];
		}

		// create empty edge_list_array of appropriate size
		create_edge_list_array(&((*dest)->edge_list_array), (*dest)->size);
	}
}

// currently creates an empty list_array
// TODO: add support with new_graph for copying in lists are creation
void create_edge_list_array(struct edge_list ** list_array, int size){
	(*list_array) = malloc(sizeof(struct edge_list) * size);

	for(int i = 0; i < size; i++){
		(*list_array)[i].num_edges = 0;
		(*list_array)[i].head = NULL;
	}
}

// add an edge to the specific list, to index
void add_edge(struct edge_list * list, int index){
	if(list->head == NULL){
		list->head = malloc(sizeof(struct edge));
		list->head->next =  NULL;
		list->head->index_of_edge = index;
		list->num_edges++;
	} else if(list->head->next == NULL){
		list->head->next = malloc(sizeof(struct edge));
		list->head->next->next = NULL;
		list->head->next->index_of_edge = index;
		list->num_edges++;
	} else {
		struct edge * cur = list->head->next;
		while(cur->next != NULL){
			cur = cur->next;
		}
		cur->next = malloc(sizeof(struct edge));
		cur->next->index_of_edge = index;
		cur->next->next = NULL;
		list->num_edges++;
	}

}
// add a value to the end of the graph, give it a blank edge_list_array entry
void add_node(struct graph * src, int value){
	// handle if empty
	if(src->size == 0 || src->values == NULL || src->edge_list_array == NULL){
		// create an empty list with size 1
		src->values = malloc(sizeof(int));
		(*src->values) = value;

		src->size = 1;
		create_edge_list_array(&(src->edge_list_array), src->size);
		
	}	

	// else resize, add nodes
	else {
		src->size++;
		src->values = realloc(src->values, sizeof(int) * src->size);	
		src->values[src->size - 1] = value;
		
		src->edge_list_array = realloc(src->edge_list_array, sizeof(struct edge_list) * src->size);
		src->edge_list_array[src->size - 1].head = NULL;
		src->edge_list_array[src->size - 1].num_edges = 0;
			
	}
	
}

// TODO: make ncurses compatible
// TODO: make safe for empty graphs
void print_graph(struct graph * src){
	for(int i = 0; i < src->size; i++){
		printf("List %d (for value %d): ", i, src->values[i]);
		struct edge * cur;
		cur = src->edge_list_array[i].head;
		while(cur != NULL){
			printf("%d ", cur->index_of_edge);
			cur = cur->next;
		}
		printf("\n");
	}
}
