#pragma once
#include "framework.h"
#include "Screen.h"
#include "Player.h"
#include "Input.h"

class GameCenter
{
	RECT windowSize;

	Screen * s;
	Player * p;
	Input * i;

public:
	GameCenter();
	~GameCenter();

	RECT GetWindowSize();

	void Update(float fElapsedTime);
	void Draw(HDC hdc);
};

