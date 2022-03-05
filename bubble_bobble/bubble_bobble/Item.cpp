#include "Item.h"

Item::Item(fPoint _pos)
{
	img = &Storage::GetInstance().GetItemImg();
	
	pos = _pos;
	readyToShowScore = false;

	SetRandomItemIdx();

	s = new Score(pos, GetItemScore(), SCORE_MOVEMENT);

}

Item::~Item() {	delete s; }

int Item::GetItemIdx() { return itemIdx; }

int Item::GetItemScore()
{
	// 10 ~ 90��
	if (itemIdx < 9) return (itemIdx + 1) * 10;
	// 100 ~ 950��
	else if (itemIdx < 27) return (itemIdx - 9) * 50 + 100;
	// 1000 ~ 6000��
	else if (itemIdx < 33) return (itemIdx - 27 + 1) * 1000;
}

bool Item::GetReadyToShowScore() { return readyToShowScore; }

void Item::SetRandomItemIdx()
{
	int num = rand() % 100 + 1;

	// 10 ~ 90�� (0~8) 9�� 80%
	if (num <= 80) itemIdx = rand() % 9;
	// 100 ~ 950�� (9~26) 18�� 15%
	else if (num <= 95) itemIdx = rand() % 18 + 9;
	// 1000 ~ 6000�� (27~32) 6�� 5%
	else itemIdx = rand() % 6 + 27;
}

void Item::SetReadyToShowScore(bool _ReadyToShowScore) { readyToShowScore = _ReadyToShowScore; }

void Item::MoveItem(float fSpeed)
{
	if (readyToShowScore == false) return;

	s->MoveScore(fSpeed);

	if (s->GetIsMoveToGoalDone() == true) isMoveToGoalDone = true;
}

void Item::DrawItem(Graphics * graphic)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(15, 79, 174), Color(15, 79, 174));
	
	if (readyToShowScore == false)
	{
		int height = (*img)[0]->GetHeight();

		POINT spos = { (int)pos.x + size * TILE_SIZE - ITME_WIDTH * 2, (int)pos.y + size * TILE_SIZE - height * 2 };
		Rect dest = { spos.x, spos.y, ITME_WIDTH * 2, height * 2 }; 

		graphic->DrawImage((*img)[0], dest, 2 * (itemIdx + 1) + ITME_WIDTH * itemIdx, 0, ITME_WIDTH, height, UnitPixel, &imgAttr);
	}
	else
	{
		s->DrawScore(graphic, 2);
	}
}