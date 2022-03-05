#pragma once
#include "Object.h"

class Player: public Object
{

public:
	Player();
	~Player();

	void MovePlayer(int moveX, int moveY, int tileSize, Screen & s, float fElapsedTime);
	void DrawPlayer(Graphics *graphic, int tileSize, FPOINT offset);
};

