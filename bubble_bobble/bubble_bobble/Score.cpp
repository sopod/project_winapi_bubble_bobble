#include "Score.h"

Score::Score()
{
	pos = { -100, -100 };
	score = 1000;
	img = &Storage::GetInstance().GetItemImg();

	SetVelWithGoal(pos);
}

Score::Score(fPoint _pos, int _score, int score_move)
{
	pos = _pos;
	score = _score;

	img = &Storage::GetInstance().GetItemImg();

	SetVelWithGoal({ pos.x, pos.y - score_move });
}

Score::~Score()
{
}

void Score::InitScore(fPoint _pos, int _score, int score_move)
{
	pos = _pos;
	score = _score;

	img = &Storage::GetInstance().GetItemImg();

	SetVelWithGoal({ pos.x, pos.y - score_move });
	isMoveToGoalDone = false;
}

int Score::GetImgSizeIdx()
{
	if (score < 100)        return 1;
	else if (score < 1000)  return 2;
	else if (score < 10000) return 3;
}


int Score::GetImgIdx()
{
	if (score < 100)        return score / 10 - 1;
	else if (score < 1000)  return score / 50 - 2;
	else if (score < 10000) return score / 1000 - 1;
}

void Score::MoveScore(float fSpeed)
{
	MoveToGoal(fSpeed, false);
}

void Score::DrawScore(Graphics * graphic, int size)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(15, 79, 174), Color(15, 79, 174));

	int idx = GetImgSizeIdx();
	int num_idx = GetImgIdx();
	int height = (*img)[idx]->GetHeight();

	int x = pos.x;
	int y = pos.y;
	if (size > 2)
	{
		x -= (IMG_DATA[idx][1] * size - IMG_DATA[idx][1] * 2) / 2;
		y -= (height * size - height * 2) / 2;
	}

	Rect dest = { x, y, IMG_DATA[idx][1] * size, height * size };
	graphic->DrawImage((*img)[idx], dest, IMG_DATA[idx][0] * (num_idx + 1) + IMG_DATA[idx][1] * num_idx, 0, IMG_DATA[idx][1], height, UnitPixel, &imgAttr);
	
}
