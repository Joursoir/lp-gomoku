#ifndef LPG_GAMEFIELD_H
#define LPG_GAMEFIELD_H

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

enum states {
	G_EMPTY = 0,
	G_NONE,
	G_DRAW,
	G_XPLAYER = 10,
	G_OPLAYER = -10
};

class GameField {
	// optional:
	int **field;
	int rows, cols;
	int free_fields;
	int win_length;

	// non-optional:
	int state;
	int who_move;
public:
	GameField(int a_cols, int a_rows, int a_lwin);
	GameField(const GameField &a);
	~GameField();
	int GetState() { return state; }
	int GetRows() { return rows; }
	int GetCols() { return cols; }

	bool CanMove(int y, int x);
	void Move(int y, int x);
	void UndoMove(int y, int x);

private:
	void UpdateState(int y, int x);
	int ScanRowsAround(int y, int x);
	int ScanColsAround(int y, int x);
	int ScanDiagsAround(int y, int x);
};

#endif /* LPG_GAMEFIELD_H */
