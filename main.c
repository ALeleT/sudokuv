#include <ncurses.h>
#include "game_loop.h"

int main(){
	int color_mode = 0;
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	if(has_colors()){
		color_mode = 1;
		start_color();
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
		init_pair(3, COLOR_WHITE, COLOR_BLACK);
	}


	sudoku_loop(color_mode);

	endwin();
}
