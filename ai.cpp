#include <stdlib.h>
#include <time.h>

#include "ai.hpp"
#include "GameField.hpp"

enum scores {
	MIN_SCORE = -1000,
	MAX_SCORE = 1000,
	NONE_SCORE = 0,
	DRAW_SCORE = 2,
	WIN_SCORE = 10,
	LOSE_SCORE = -10
};

AI::AI(int p, int d) : player(p), max_depth(d), first_move(true)
{
	srand(time(NULL));
}

void AI::GetBestMove(int &my, int &mx, GameField field)
{
	int score = MIN_SCORE;
	int rows = field.GetRows();
	int cols = field.GetCols();

	// give chance to user :)
	if(first_move) {
		my = rand() % rows;
		mx = rand() % cols;
		first_move = false;
		return;
	}

	int y, x;
	for(y = 0; y < rows; y++) {
		for(x = 0; x < cols; x++)
		{
			if(!field.CanMove(y, x))
				continue;
			field.Move(y, x);
			int result = min(field, 1);
			field.UndoMove(y, x);
			if(result > score) {
				score = result;
				my = y;
				mx = x;
			}
		}
	}
}

int AI::score(GameField field)
{
	int state = field.GetState();
	if(state == G_DRAW)
		return DRAW_SCORE;
	if(state == G_XPLAYER)
		return player == G_XPLAYER ? WIN_SCORE : LOSE_SCORE;
	if(state == G_OPLAYER)
		return player == G_OPLAYER ? WIN_SCORE : LOSE_SCORE;
	return NONE_SCORE;
}

int AI::min(GameField field, int depth)
{
	if(field.GetState() != G_NONE || depth >= max_depth) {
		return score(field);
	}

	int score = MAX_SCORE;
	int rows = field.GetRows();
	int cols = field.GetCols();

	int y, x;
	for(y = 0; y < rows; y++) {
		for(x = 0; x < cols; x++)
		{
			if(!field.CanMove(y, x))
				continue;
			field.Move(y, x);
			int result = max(field, depth + 1);
			field.UndoMove(y, x);
			if(result < score)
				score = result;
		}
	}

	return score;
}

int AI::max(GameField field, int depth)
{
	if(field.GetState() != G_NONE || depth >= max_depth) {
		return score(field);
	}

	int score = MIN_SCORE;
	int rows = field.GetRows();
	int cols = field.GetCols();

	int y, x;
	for(y = 0; y < rows; y++) {
		for(x = 0; x < cols; x++)
		{
			if(!field.CanMove(y, x))
				continue;
			field.Move(y, x);
			int result = min(field, depth + 1);
			field.UndoMove(y, x);
			if(result > score)
				score = result;
		}
	}

	return score;
}
