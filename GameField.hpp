#ifndef LPG_GAMEFIELD_H
#define LPG_GAMEFIELD_H

enum states {
	G_EMPTY = 0,
	G_NONE,
	G_DRAW,
	G_XPLAYER = 10,
	G_OPLAYER = -10
};

class GameField {
	// optional:
	int field[3][3];
	int free;
	int win_length;

	// non-optional:
	int state;
	int who_move;
public:
	GameField() : field{0}, free(9), win_length(3), state(G_NONE),
		who_move(G_XPLAYER) { }
	~GameField() { }
	int GetState() { return state; }

	bool CanMove(int x, int y);
	void Move(int x, int y);

private:
	void UpdateState();
	int ScanRows();
	int ScanCols();
	int ScanDiags();
};

#endif /* LPG_GAMEFIELD_H */
