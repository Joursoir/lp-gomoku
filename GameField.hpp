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
	int **field;
	int cols, rows;
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
	int GetCols() { return cols; }
	int GetRows() { return rows; }

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
