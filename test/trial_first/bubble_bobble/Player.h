#pragma once
#include "Object.h"

class Player : public Object
{
protected:

public:
	Player(int tileSize);
	~Player();

	virtual void DrawObject(HDC hdc, int tileSize);
	//void MoveObject(float fSpeed, std::pair<int, int> rangeXPos, Map & m, int tileSize, int moveX, int moveY);
	void MoveObject(float fSpeed, std::pair<int, int> rangeXPos, std::vector<FPOINT> & coll, int tileSize, int moveX, int moveY);
	virtual void DeleteObject();
};

