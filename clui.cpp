/***
	This file is part of Let's play gomoku!
	Copyright (C) 2021 Aleksandr D. Goncharov (Joursoir) <chat@joursoir.net>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
***/

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdarg.h>

#include "GameField.hpp"
#include "ai.hpp"

#define MIN_RCWD 3 // RCWD = rows/cols/win/depth
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
AI *game_bot;
bool play_with_ai = false;

/* players vars */
int cursor_y, cursor_x;
char print_symbol;

/* command line options */
int gb_y = MIN_RCWD;
int gb_x = MIN_RCWD; 
int gb_lwin = MIN_RCWD;
int gb_symbol = SYMBOL_PLAYERONE;
int gb_depth = MIN_RCWD;

void aiMove();

void updateCursor()
{
	move(cursor_y, cursor_x);
	refresh();
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

void startGame()
{
	if(game_field)
		delete game_field;
	game_field = new GameField(gb_y, gb_x, gb_lwin);
	drawGame(gb_y, gb_x);
	if(play_with_ai && gb_symbol == SYMBOL_PLAYERTWO) {
		game_bot->FirstMove(true);
		aiMove();
	}
}

void changePlayer()
{
	if(print_symbol == SYMBOL_PLAYERONE)
		print_symbol = SYMBOL_PLAYERTWO;
	else
		print_symbol = SYMBOL_PLAYERONE;
	help_print("MOVE: %c", print_symbol);
}

void printMove(int y, int x, int symbol)
{
	move(y, x);
	addch(symbol);
	updateCursor();
}

void gameMove(int y, int x)
{
	int state = game_field->GetState();
	if(state != G_NONE)
		return;

	int game_y = (y - min_y) / NC_MOVE_Y;
	int game_x = (x - min_x) / NC_MOVE_X;

	if(!game_field->CanMove(game_y, game_x)) 
		return;
	
	game_field->Move(game_y, game_x);
	printMove(y, x, print_symbol);
	changePlayer();

	state = game_field->GetState();
	if(state == G_NONE) {
		if(play_with_ai && gb_symbol != print_symbol)
			aiMove();
	}
	else if(state == G_DRAW)
		help_print("DRAW!");
	else if(state == G_XPLAYER)
		help_print("WINNER: %c!", SYMBOL_PLAYERONE);
	else if(state == G_OPLAYER)
		help_print("WINNER: %c!", SYMBOL_PLAYERTWO);
}

void aiMove()
{
	int y, x;
	game_bot->GetBestMove(y, x, *game_field);
	gameMove(min_y + NC_MOVE_Y * y, min_x + NC_MOVE_X * x);
}

void handleButtons()
{
	int ch;
	while((ch = getch()) != key_escape && ch != KEY_RESIZE)
	{
		switch(ch) {
		case KEY_LEFT:
			if(cursor_x > min_x)
				cursor_x -= NC_MOVE_X;
			break;
		case KEY_DOWN:
			if(cursor_y < max_y)
				cursor_y += NC_MOVE_Y;
			break;
		case KEY_UP:
			if(cursor_y > min_y)
				cursor_y -= NC_MOVE_Y;
			break;
		case KEY_RIGHT:
			if(cursor_x < max_x)
				cursor_x += NC_MOVE_X; 
			break;
		case key_enter:
			gameMove(cursor_y, cursor_x);
			break;
		case key_restart:
			startGame();
			// no break
		default:
			continue;
		}
		updateCursor();
	}
}

void usage()
{
	endwin();
	printf("Let's play gomoku!\n"
		"Synopsis:\n"
		"\tlpgomoku [OPTION]\n"
		"Option:\n"
		"\t-h, --help\n"
		"\t\tPrint this text.\n"
		"\t-v, --version\n"
		"\t\tPrint version information.\n"
		"\t-r, --rows\n"
		"\t\tNumber of rows. %d is used by default\n"
		"\t-c, --cols\n"
		"\t\tNumber of columns. %d is used by default\n"
		"\t-w, --win\n"
		"\t\tLength of winning combination. %d is used by default\n"
		"\t-a, --ai\n"
		"\t\tArtificial intelligence that play with you. Disabled by default\n"
		"\t-s, --symbol\n"
		"\t\tChoose a mark for move. Available only if option --ai enabled.\n"
		"\t-d, --depth\n"
		"\t\tDepth of the game tree for AI. Available only if option "
		"--ai enabled. %d is used by default\n",
		MIN_RCWD, MIN_RCWD, MIN_RCWD, MIN_RCWD);
}

void version()
{
	endwin();
	printf("Let's play gomoku!\n"
		"Copyright (C) 2021 Aleksandr D. Goncharov\n"
		"License: GNU GPL version 3\n"
		"This is free software: you are free to change and redistribute it.\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n");
}

int main(int argc, char *argv[])
{
	/* ncurses settings */
	initscr();
	keypad(stdscr, TRUE);
	noecho();

	/* command line options */
	const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"rows", required_argument, NULL, 'r'},
		{"cols", required_argument, NULL, 'c'},
		{"win", required_argument, NULL, 'w'},
		{"ai", no_argument, NULL, 'a'},
		{"symbol", required_argument, NULL, 's'},
		{"depth", required_argument, NULL, 'd'},
		{NULL, 0, NULL, 0}
	};

	int result;
	while((result = getopt_long(argc, argv, "hvr:c:w:as:d:", long_options, NULL)) != -1) {
		switch(result) {
		case 'h': { usage(); return 0; }
		case 'v': { version(); return 0; }
		case 'r': { gb_y = atoi(optarg); break; }
		case 'c': { gb_x = atoi(optarg); break; }
		case 'w': { gb_lwin = atoi(optarg); break; }
		case 'a': { play_with_ai = true; break; }
		case 's': { gb_symbol = optarg[0]; break; }
		case 'd': { gb_depth = atoi(optarg); break; }
		default: break;
		}
	}

	if(gb_y < MIN_RCWD || gb_x < MIN_RCWD || gb_lwin < MIN_RCWD || gb_depth < MIN_RCWD) {
		endwin();
		printf("Valid columns/rows/win/depth length are %d and more\n", MIN_RCWD);
		return 1;
	}
	if(play_with_ai) {
		if(gb_symbol != SYMBOL_PLAYERONE && gb_symbol != SYMBOL_PLAYERTWO) {
			endwin();
			printf("Valid symbol: %c (move first), %c\n",
				SYMBOL_PLAYERONE, SYMBOL_PLAYERTWO);
			return 1;
		}
		game_bot = new AI(gb_symbol == SYMBOL_PLAYERONE ?
			G_OPLAYER : G_XPLAYER, gb_depth);
	}
	else gb_symbol = SYMBOL_PLAYERONE;

	startGame();
	handleButtons();

	if(game_field)
		delete game_field;
	if(game_bot)
		delete game_bot;
	endwin();
	return 0;
}
