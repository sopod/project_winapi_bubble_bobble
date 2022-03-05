#pragma once
#include "framework.h"
#include "Object.h"

class Score : public Object
{
protected:
	int score;

public:
	Score();
	Score(fPoint pos, int score, int scoreMove);
	~Score();

	void InitScore(fPoint pos, int score, int scoreMove);

	int GetImgSizeIdx();
	int GetImgIdx();

	void MoveScore(float speed);

	void DrawScore(Graphics * graphic, int size);
};

