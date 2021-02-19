#include "GameField.hpp"

GameField::GameField(int a_rows, int a_cols, int a_lwin)
	: rows(a_rows), cols(a_cols), win_length(a_lwin), state(G_NONE),
	who_move(G_XPLAYER)
{
	int r, c;
	field = new int*[rows];
	for(r = 0; r < rows; r++) {
		field[r] = new int[cols];
		for(c = 0; c < cols; c++) {
			field[r][c] = G_EMPTY;
		}
	}
	free_fields = rows * cols;
}

GameField::GameField(const GameField& a)
{
	int r, c;
	rows = a.rows;
	cols = a.cols;
	free_fields = a.free_fields;
	win_length = a.win_length;
	state = a.state;
	who_move = a.who_move;

	field = new int*[rows];
	for(r = 0; r < rows; r++) {
		field[r] = new int[cols];
		for(c = 0; c < cols; c++) {
			field[r][c] = a.field[r][c];
		}
	}
}

GameField::~GameField()
{
	if(field) {
		int r;
		for(r = 0; r < rows; r++) {
			delete[] field[r];
		}
		delete[] field;
	}
}

bool GameField::CanMove(int y, int x)
{
	if(field[y][x] != G_EMPTY)
		return false;
	return true;
}

void GameField::Move(int y, int x)
{
	field[y][x] = who_move;
	who_move = -who_move; // change player
	free_fields--;

	UpdateState(y, x);
}

void GameField::UndoMove(int y, int x)
{
	who_move = field[y][x];
	field[y][x] = G_EMPTY;
	free_fields++;

	UpdateState(y, x);
}

void GameField::UpdateState(int y, int x) 
{
	if((state = ScanRowsAround(y, x)) != G_NONE)
		return;
	if((state = ScanColsAround(y, x)) != G_NONE)
		return;
	if((state = ScanDiagsAround(y, x)) != G_NONE)
		return;

	if(!free_fields) {
		state = G_DRAW;
		return;
	}
}

int GameField::ScanRowsAround(int y, int x)
{
	int i, n;
	int player = field[y][x];
	// ray to left
	for(i = x, n = 0; i >= 0; i--) {
		if(player == field[y][i]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}

	// ray to right
	for(i = x, n -= 1; i < cols; i++) {
		if(player == field[y][i]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}

	return G_NONE;
}

int GameField::ScanColsAround(int y, int x)
{
	int i, n;
	int player = field[y][x];
	// ray to up
	for(i = y, n = 0; i >= 0; i--) {
		if(player == field[i][x]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}

	// ray to down
	for(i = y, n -= 1; i < rows; i++) {
		if(player == field[i][x]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}

	return G_NONE;
}

int GameField::ScanDiagsAround(int y, int x)
{
	int i, j, n;
	int player = field[y][x];
	// ray to left up corner
	for(i = x, j = y, n = 0; i >= 0 && j >= 0; i--, j--) {
		if(player == field[j][i]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}

	// ray to right down corner
	for(i = x, j = y, n -= 1; i < cols && j < rows; i++, j++) {
		if(player == field[j][i]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}
	
	/* change the diagonal */

	// ray to right up corner
	for(i = x, j = y, n = 0; i < cols && j >= 0; i++, j--) {
		if(player == field[j][i]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}

	// ray to left down cornet
	for(i = x, j = y, n -= 1; i >= 0 && j < rows; i--, j++) {
		if(player == field[j][i]) {
			n++;
			if(n == win_length)
				return player;
		}
		else break;
	}

	return G_NONE;
}
