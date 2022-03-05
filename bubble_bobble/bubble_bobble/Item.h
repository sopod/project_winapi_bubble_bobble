#pragma once
#include "framework.h"
#include "Object.h"
#include "Score.h"

class Item : public Object
{
private:
	int itemIdx;

	Score * s;
	bool readyToShowScore;

public:
	Item(fPoint pos);
	~Item();
	
	int GetItemIdx();
	int GetItemScore();
	bool GetReadyToShowScore();

	void SetRandomItemIdx();
	void SetReadyToShowScore(bool readyToShowScore);

	void MoveItem(float speed);

	void DrawItem(Graphics * graphic);
};

