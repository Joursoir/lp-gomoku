#ifndef LPG_AI_H
#define LPG_AI_H

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
