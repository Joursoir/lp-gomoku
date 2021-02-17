#include "GameField.hpp"

bool GameField::CanMove(int x, int y)
{
	if(field[y][x] != G_EMPTY)
		return false;
	return true;
}

void GameField::Move(int x, int y)
{
	field[y][x] = who_move;
	who_move = -who_move; // change player
	free--;

	UpdateState();
}

void GameField::UpdateState() 
{
	int tmp;

	if(!free) {
		state = G_DRAW;
		return;
	}
	tmp = ScanRows();
	if(tmp) {
		state = tmp;
		return;
	}
	tmp = ScanCols();
	if(tmp) {
		state = tmp;
		return;
	}
	tmp = ScanDiags();
	if(tmp) {
		state = tmp;
		return;
	}
}

/* This version is a dummy */
int GameField::ScanRows()
{
	int i;
	for(i = 0; i < 3; i++) {
		if(field[i][0] == field[i][1] && field[i][1] == field[i][2] &&
				field[i][1] != G_EMPTY)
			return field[i][1];
	}

	return 0;
}

/* This version is a dummy */ 
int GameField::ScanCols()
{
	int i;
	for(i = 0; i < 3; i++) {
		if(field[0][i] == field[1][i] && field[1][i] == field[2][i] &&
				field[1][i] != G_EMPTY)
			return field[1][i];
	}

	return 0;
}

/* This version is a dummy */
int GameField::ScanDiags()
{
	if(field[0][0] == field[1][1] && field[1][1] == field[2][2] &&
			field[1][1] != G_EMPTY)
		return field[1][1];
	else if(field[0][2] == field[1][1] && field[1][1] == field[2][0] &&
			field[1][1] != G_EMPTY)
		return field[1][1];

	return 0;
}
