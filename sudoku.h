#ifndef SUDOKU_H
#define SUDOKU_H
#include "graph.h"

struct sudoku {
	struct graph * board;
	int * valid_arr;
};

void free_sudoku(struct sudoku ** target);
int attempt_move(struct sudoku * dest, int curs_row, int curs_col, int input);
int check_index(struct sudoku * src, int index, int mode);
int check_win(struct sudoku * src);

int solve_sudoku(struct sudoku * game);
struct sudoku * new_sudoku(int difficulty);
// populate builds the graph for checking validity
struct sudoku * populate();
#endif
