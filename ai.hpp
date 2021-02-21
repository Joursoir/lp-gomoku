#ifndef LPG_AI_H
#define LPG_AI_H

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

class GameField;

class AI {
	int player;
	int max_depth;
	bool first_move;
public:
	AI(int p, int d);

	void GetBestMove(int &my, int &mx, GameField field);
	void FirstMove(bool m) { first_move = m; }
private:
	int score(GameField field);
	int MinMax(GameField field, int alpha, int beta, int depth);
};

#endif /* LPG_AI_H */
