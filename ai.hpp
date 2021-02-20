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
private:
	int score(GameField field);
	int min(GameField field, int depth);
	int max(GameField field, int depth);
};

#endif /* LPG_AI_H */
