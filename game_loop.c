#include <ncurses.h>
#include "game_loop.h"
#include "sudoku.h"

// options is an array of strings, size is the number of strings
// must be null-terminated strings
int print_menu(const char options[][MAX_MENU], int size){
	clear();
	for(int i = 0; i < size; i++)
		mvprintw(i,0,"%s", options[i]);	
	refresh();
	return 1;
}

// prints a single char at the given row/col and with color mode, and color flag
void print_mode_item(int row, int col, int ch, int color_mode, int color_flag){
	if(color_mode == 1){
		attron(COLOR_PAIR(color_flag));
	}
	
		mvprintw(row, col, "%c", ch);
		refresh();
	if(color_mode == 1){
		attroff(COLOR_PAIR(color_flag));
	}
}

// prints the given sudoku board, option for colors or not
void print_sudoku(struct sudoku * game, int color_mode){
	clear();
	int valDex = 0;
	for(int row = 0; row < 9; row++){
		for(int col = 0; col < 9; col++){
			if(game->board->values[valDex] == 0){
				print_mode_item(row, col, '-', color_mode, 3);	
			}
			else{
				if(game->valid_arr[valDex] == 0 || game->valid_arr[valDex] == 3){
					print_mode_item(row, col, '0' + game->board->values[valDex], color_mode, 3); 
				}	
				else if(game->valid_arr[valDex] == 1){
					print_mode_item(row, col, '0' + game->board->values[valDex], color_mode, 1);
				}
				else if(game->valid_arr[valDex] == 2){
					print_mode_item(row, col, '0' + game->board->values[valDex], color_mode, 2);
				}
			}

			valDex++;
		}

	}
}

// PLAY, start playing with the sudoku board given
// if game is NULL, we instead generate a board randomly
// and play that
void play(struct sudoku * game, int color_mode){
	int curs_row = 0, curs_col = 0;
	if(game == NULL){
		game = new_sudoku(0); // handle difficulty later
	}
	while(!check_win(game)){
		print_sudoku(game, color_mode);
		move(curs_row, curs_col);
		int input = getch();
		switch(input){
			case KEY_UP:
			if(curs_row > 0){
				curs_row--;
			}
			break;
			case KEY_DOWN:
			if(curs_row < 8){
				curs_row++;
			}
			break;
			case KEY_LEFT:
			if(curs_col > 0){
				curs_col--;
			}
			break;
			case KEY_RIGHT:
			if(curs_col < 8){
				curs_col++;
			}
			break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			attempt_move(game, curs_row, curs_col, (int)(input-'0'));
			break;
			case KEY_BACKSPACE:
			attempt_move(game, curs_row, curs_col, 0);
			break;
			default:
			break;
		}
	}

}

// allow the player to input a sudoku puzzle
// take the puzzle and pass it to our solve
// display the result
void solve_and_display(struct sudoku * game, int color_mode){
	if(game == NULL)	
		game = new_sudoku(0);

	int input = 0;
	int curs_row = 0, curs_col = 0;
	while(input != 'q'){
		print_sudoku(game, color_mode);
		move(curs_row, curs_col);
		input = getch();
		switch(input){
			case KEY_UP:
			if(curs_row > 0){
				curs_row--;
			}
			break;
			case KEY_DOWN:
			if(curs_row < 8){
				curs_row++;
			}
			break;
			case KEY_LEFT:
			if(curs_col > 0){
				curs_col--;
			}
			break;
			case KEY_RIGHT:
			if(curs_col < 8){
				curs_col++;
			}
			break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			attempt_move(game, curs_row, curs_col, (int)(input-'0'));
			break;
			case KEY_BACKSPACE:
			attempt_move(game, curs_row, curs_col, 0);
			break;
			default:
			break;
		}
	}
	// set everything in the valid arr that is 1 to 3, a given
	for(int i = 0; i < 81; i++)
		if(game->valid_arr[i] == 1)
			game->valid_arr[i] = 3;

	// now that we are outside the building loop..
	solve_sudoku(game);
	print_sudoku(game, color_mode);
	int read = 0;
	while((read = getch()) != 'c');	
}

// menu loop 
int sudoku_loop(int color_mode){
	int option = 0;
	while(print_menu(main_menu_sudoku, MAIN_MENU_ITEMS) && (option = getch()) && option != 'q'){
		switch(option){
			case '1':
				play(NULL, color_mode);
				break;
			case '2':
				solve_and_display(NULL, color_mode);
				break;
			default:
				break;
		}			
	}	
	
	return option;
}

