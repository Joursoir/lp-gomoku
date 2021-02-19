#ifndef LPG_AI_H
#define LPG_AI_H

class GameField;

class AI {
	int max_depth;
public:
	AI(int d);

	void GetFirstMove(int &my, int &mx, int rows, int cols);
	void GetBestMove(int &my, int &mx, GameField field);
private:
	int score(GameField field);
	int min(GameField field, int depth);
	int max(GameField field, int depth);
};

#endif /* LPG_AI_H */
