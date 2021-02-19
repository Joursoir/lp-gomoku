#include <ncurses.h>
#include <string.h>
#include <stdarg.h>

#include "GameField.hpp"

#define NC_MOVE_X 2
#define NC_MOVE_Y 2
#define SYMBOL_PLAYERONE 'x'
#define SYMBOL_PLAYERTWO 'o'

const char app_name[] = "Let's play gomoku!";
const char app_tips[] = "R - restart game; ESC - exit";

enum keys {
	key_restart = 'r',
	key_escape = 27,
	key_enter = 10
};

/* clui vars */
int screen_rows = 0, screen_cols = 0;
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
	move(20, 0);
	clrtoeol(); // clear old
	mvprintw(20, 0, msg);
	updateCursor();  // return cursor back
}

void help_print(const char *msg, ...)
{
	move(max_y + 2, 0);
	clrtoeol();

	va_list args;
	va_start(args, msg);
	char *str = new char[screen_cols+1];
	vsprintf(str, msg, args);
	va_end(args);

	mvprintw(max_y + 2, (screen_cols - strlen(str)) / 2, str);
	updateCursor();
	delete[] str;
}

void drawGame(int rows, int cols)
{
	getmaxyx(stdscr, screen_rows, screen_cols);

	int used_row = rows * NC_MOVE_Y - 1;
	min_y = (screen_rows - used_row) / 2;
	max_y = min_y + used_row - 1;

	int used_col = cols * NC_MOVE_X - 1;
	min_x = (screen_cols - used_col) / 2;
	max_x = min_x + used_col - 1;

	int i, j, d;
	for(j = min_y, d = 0; j <= max_y; j++, d++) {
		move(j, min_x);
		if(d % 2 == 0) {
			addch('#');
			for(i = 1; i < cols; i++) {
				addch('|');
				addch('#');
			}
		}
		else {
			addch('-');
			for(i = 1; i < cols; i++) {
				addch('|');
				addch('-');
			}
		}
	}

	mvprintw(min_y - 3, (screen_cols - strlen(app_name)) / 2, app_name);
	mvprintw(min_y - 2, (screen_cols - strlen(app_tips)) / 2, app_tips);
	help_print("MOVE: %c", SYMBOL_PLAYERONE);

	cursor_y = min_y; // + (max_y - min_y) / NC_MOVE_Y;
	cursor_x = min_x; // + (max_x - min_x) / NC_MOVE_X;
	print_symbol = SYMBOL_PLAYERONE;
	updateCursor();
}

void changePlayer()
{
	if(player_symbol == SYMBOL_PLAYERONE)
		player_symbol = SYMBOL_PLAYERTWO;
	else
		player_symbol = SYMBOL_PLAYERONE;
	help_print("MOVE: %c", player_symbol);
}

void printMove(int y, int x, int symbol)
{
	move(y, x);
	addch(symbol);
	updateCursor();
}

void gameMove()
{
	int state = game_field->GetState();
	if(state != G_NONE)
		return;

	int game_y = (cursor_y - min_y) / NC_MOVE_Y;
	int game_x = (cursor_x - min_x) / NC_MOVE_X;

	if(!game_field->CanMove(game_y, game_x)) {
		dbgprint("yet 'x' or 'o'");
		return;
	}
	game_field->Move(game_y, game_x);
	printMove(cursor_y, cursor_x, player_symbol);
	changePlayer();

	state = game_field->GetState();
	if(state == G_NONE) {
		/* if play with AI:
		ai move */
	}
	else if(state == G_DRAW)
		help_print("DRAW!");
	else if(state == G_XPLAYER)
		help_print("WINNER: %c!", SYMBOL_PLAYERONE);
	else if(state == G_OPLAYER)
		help_print("WINNER: %c!", SYMBOL_PLAYERTWO);
}

void handleButtons()
{
	int ch;
	while((ch = getch()) != key_escape && ch != KEY_RESIZE)
	{
		switch(ch) {
		case KEY_LEFT:
			if(cursor_x > min_x) cursor_x -= NC_MOVE_X;
			else dbgprint("left border");
			break;
		case KEY_DOWN:
			if(cursor_y < max_y) cursor_y += NC_MOVE_Y;
			else dbgprint("down border");
			break;
		case KEY_UP:
			if(cursor_y > min_y) cursor_y -= NC_MOVE_Y;
			else dbgprint("up border");
			break;
		case KEY_RIGHT:
			if(cursor_x < max_x) cursor_x += NC_MOVE_X; 
			else dbgprint("right border");
			break;
		case key_enter: {
			gameMove();
			break;
		}
		case key_restart: {
			delete game_field;
			drawGame(gb_y, gb_x);
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
	drawGame(gb_y, gb_x);
	handleButtons();

	if(game_field)
		delete game_field;
	endwin();
	return 0;
}
