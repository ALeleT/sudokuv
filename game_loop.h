#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "sudoku.h"
#define MAX_MENU 256
#define MAIN_MENU_ITEMS 4
static const char main_menu_sudoku[MAIN_MENU_ITEMS][MAX_MENU] = {{"Sudoku Engine"},
			    {"1) Generate Puzzle"},
			    {"2) Solve Puzzle"},
			    {"q) Quit"}};
void print_sudoku(struct sudoku * game, int color_mode);
int sudoku_loop(int color_mode);
#endif
