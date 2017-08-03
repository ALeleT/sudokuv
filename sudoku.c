#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sudoku.h"

// for player input
int attempt_move(struct sudoku * dest, int curs_row, int curs_col, int input){
	int move  = 0;
	int index = (curs_row * 9) + curs_col;
	// if the cell is holding a zero, or marked incorrectly, let the move be made
	if(dest->board->values[index] == 0 || dest->valid_arr[index] == 2 || dest->valid_arr[index] == 1){
		dest->board->values[index] = input;
		check_index(dest, index, 0); // don't care if it succeeds
		move = 1;
	}
	// allow the zeroing of a cell with attempt move
	if(input == 0 && (dest->valid_arr[index] == 2 || dest->valid_arr[index] == 1)){
		dest->board->values[index] = 0;
		check_index(dest, index, 0);
		// check all indexes to shift to correct states
		for(int i = 0; i < 81; i++){
			if(dest->valid_arr[i] == 2)
				check_index(dest, i, dest->board->values[i]);
		}
		move = 1;
	}

	return move;
}


// checks if the fed index is valid
int check_index(struct sudoku * game, int index, int mode){
	int valid = 1;
	struct edge * cur = game->board->edge_list_array[index].head;
	while(cur != NULL){
		if(game->board->values[cur->index_of_edge] == game->board->values[index]){
			valid = 2;
			// they are the same value
			// unless the "hit" is a hint, mark it wrong too
			// don't mark the other wrong if we are generating
			if(game->valid_arr[cur->index_of_edge] != 3 && mode != 1)
				game->valid_arr[cur->index_of_edge] = valid;
		}
		cur = cur->next;
	}
	game->valid_arr[index] = valid;
	// return what we marked the index as
	return valid;
}

// check the condition of valid array, if not solved or given, fail
int check_win(struct sudoku * game){
	int completed = 1;
	for(int i = 0; i < 81; i++){
		// 1 is correct, 3 is a given
		// 2 is wrong 
		if(game->valid_arr[i] != 1 &&  game->valid_arr[i] != 3)
			completed = 0;
	}

	return completed;
}

// solve given sudoku, method will generate a random new sudoku if blank
int solve_sudoku(struct sudoku * game){
	int solved = 0;
	int tried_array[81][9];
	// TODO: copy over solver	

	// zero tried_array
	for(int i = 0; i < 81; i++)
		for(int j = 0; j < 9; j++)
			tried_array[i][j] = 0;

	srand((unsigned int)time(NULL));

	int insert;
	int index = 0;
	int noValid = 0;
	while(!check_win(game)){
		// iterate to first non-hint cell
		while(game->valid_arr[index] == 3)
			index++;

		// generate an unused number
		insert  = rand() % 9 + 1;
		int initial = -1;
		noValid = 0;
		// maybe this doesn't work
		while(tried_array[index][insert-1] != 0 && noValid == 0){
			if(insert  == initial)
				noValid = 1;
			if(initial == -1)
				initial = insert;
			insert++;
			if(insert > 9)
				insert = 1;
		}
		// check if it works
		game->board->values[index] = insert;
		tried_array[index][insert-1] = 1;
		check_index(game, index, 1);
		
		while(game->valid_arr[index] != 1 && noValid == 0){
			insert = random() % 9 + 1;
			initial = -1;
			while(tried_array[index][insert-1] != 0 && noValid == 0){
				if(insert == initial)
					noValid = 1;
				if(initial == -1)
					initial = insert;
				insert++;
				if(insert > 9){
					insert = 1;
				}
			}
			game->board->values[index] = insert;
			tried_array[index][insert - 1] = 1;
			check_index(game, index, 1);
		}
		
		
		// if not valid, no numbers left, roll back to first non-hint and zero tried-array, try from the top
		if(game->valid_arr[index] != 1 && noValid == 1){
			// zero current array...
			for(int i = 0; i < 9; i++){
				tried_array[index][i] = 0;
			}
			game->board->values[index] = 0;
			index--;

			// iterate backwards index until first non-hint
			while(game->valid_arr[index] == 3)
				index--;	
		} // else increment index
		else {
			index++;
		}
//		print_sudoku(game, 1);
//		getch();
		
	}
	// convert all valid_arr to hints
	for(int i = 0; i < 81; i++){
		game->valid_arr[i] = 3;
	}
	return solved;	
}

// called to generate a solvable puzzle
struct sudoku * new_sudoku(int difficulty){
	// return new sudoku with hints for player to solve
	struct sudoku * game = NULL;
	game = populate();

	// TODO:here shave down hints
	switch(difficulty){
		case 0:
			//blank
			break;
		case 1:
			//easy
			
			solve_sudoku(game);
			break;
		default:
			break;

	}	
	return game;
}

// allocates memory for a sudoku struct, allocates valid array for 81, zeroes it
// creates "graphs" between cells that should have edges 
struct sudoku * populate(){
	struct sudoku * game = NULL;

	game = malloc(sizeof(struct sudoku));
	new_graph(&(game->board), 0, NULL);
	// zero the array that holds state about correct or incorrect for each node
	game->valid_arr = calloc(81, sizeof(int));
	// create nodes for our sudoku graph, default them to 0 (empty)
	for(int i = 0; i < 81; i++){
		add_node(game->board, 0);
	}
	// build the graph connecting each node, currently they all have 0 edges
	for(int i = 0; i < 81; i++){
		// add edges for col members
		for(int j = 0; j < 9; j++){
			int colDex = (i % 9) + (j * 9);
			if(colDex != i)
				add_edge(&(game->board->edge_list_array[i]), colDex);
		}

		// add edges for row members
		for(int j = 0; j < 9; j++){
			int rowDex = 0;
			double index = i;
			double row = j;

			double edge = floor(index / (double)9) * 9.0 + row;
			rowDex = (int) edge;
			if(rowDex != i)
				add_edge(&(game->board->edge_list_array[i]), rowDex);
		}

		// add edges for block members
		int col = i % 9;
		double row = floor((double)i / (double)9);
		double col_group = floor(((double)col * 3)/9);
		double row_group = floor((row * 3)/9);

		for(int r = 0; r < 3; r++){
			for(int c = 0; c < 3; c++){
				int blockDex = (r * 9 + row_group * 27) + (c + col_group * 3);
				if(blockDex != i)
					add_edge(&(game->board->edge_list_array[i]), blockDex);
			}
		}
		
	}

	return game;
}

void free_sudoku(struct sudoku ** target){
	free_graph(&((*target)->board));
	free((*target)->valid_arr);
	free(*target);	
}	
