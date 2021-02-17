#include <ncurses.h>

#include "GameField.hpp"

#define SYMBOL_PLAYERONE 'x'
#define SYMBOL_PLAYERTWO 'o'

enum keys {
	key_restart = 'r',
	key_escape = 27,
	key_enter = 10
};

/* clui vars */
int min_y, max_y, min_x, max_x;

/* game vars */
GameField *game_field;

/* players vars */
int cursor_y, cursor_x;
char player_symbol;

/* command line options */
int gb_y = 3;
int gb_x = 3; 
int gb_lwin = 3;
int gb_symbol = SYMBOL_PLAYERONE;

void updateCursor()
{
	move(cursor_y, cursor_x);
	refresh();
}

void dbgprint(const char *msg)
{
	mvprintw(10, 0, "                                 "); // clear old
	mvprintw(10, 0, msg);
	updateCursor();  // return cursor back
	refresh();
}

void drawGame(int cols, int rows, int symbol)
{
	/* MAKE: draw for custom cols and rows */
	mvprintw(0, 0, "012");
	mvprintw(1, 0, "345");
	mvprintw(2, 0, "678");

	min_y = 0;
	max_y = cols-1;
	min_x = 0;
	max_x = rows-1; 

	cursor_y = max_y / 2;
	cursor_x = max_x / 2;
	player_symbol = symbol;
	updateCursor();
}

void changePlayer()
{
	if(player_symbol == SYMBOL_PLAYERONE)
		player_symbol = SYMBOL_PLAYERTWO;
	else
		player_symbol = SYMBOL_PLAYERONE;
}

void printMove(int y, int x, int symbol)
{
	move(y, x);
	addch(symbol);
	updateCursor();
}

void handleMove()
{
	if(!game_field->CanMove(cursor_y, cursor_x)) {
		dbgprint("yet 'x' or 'o'");
		return;
	}
	game_field->Move(cursor_y, cursor_x);
	printMove(cursor_y, cursor_x, player_symbol);
	changePlayer();

	int state = game_field->GetState();
	if(state == G_NONE) {
		/* if play with AI:
		ai move */
	}
	else if(state == G_DRAW)
		dbgprint("DRAW!");
	else if(state == G_XPLAYER)
		dbgprint("PLAYER 'X' WIN!");
	else if(state == G_OPLAYER)
		dbgprint("PLAYER 'O' WIN!");
}

void handleButtons()
{
	int ch;
	while((ch = getch()) != key_escape)
	{
		switch(ch)
		{
		case KEY_LEFT:
			if(cursor_x != min_x) {
				cursor_x -= 1;
			}
			else dbgprint("left border");
			break;
		case KEY_DOWN:
			if(cursor_y != max_y) {
				cursor_y += 1;
			}
			else dbgprint("down border");
			break;
		case KEY_UP:
			if(cursor_y != min_y) {
				cursor_y -= 1;
			}
			else dbgprint("up border");
			break;
		case KEY_RIGHT:
			if(cursor_x != max_x) {
				cursor_x += 1;
			}
			else dbgprint("right border");
			break;
		case key_enter: {
			handleMove();
			break;
		}
		case key_restart: {
			delete game_field;
			drawGame(gb_y, gb_x, gb_symbol);
			game_field = new GameField(gb_y, gb_x, gb_lwin);
			// no break
		}
		default:
			continue;
		}
		updateCursor();
	}
}

int main(int argc, char *argv[])
{
	/* ncurses settings */
	initscr();
	keypad(stdscr, TRUE);
	noecho();

	/* command line options */
	/* ... */

	game_field = new GameField(gb_y, gb_x, gb_lwin);
	drawGame(gb_y, gb_x, gb_symbol);
	handleButtons();

	if(game_field)
		delete game_field;
	endwin();
	return 0;
}
