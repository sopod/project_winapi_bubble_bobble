#pragma once
#include "framework.h"
#include <string>
#include "Player.h"
#include "Map.h"
#include "Input.h"
//#include "InputManager.h"

enum SCREEN 
{
	Lobby, Intro, Game, Score, Gameover
};

struct USER
{
	int score;
	std::string id;
};



class GameManager
{
protected:
	SCREEN curScene;
	int tileSize;

	int p1Score;
	Player * p;
	Map * m;
	Input * i;

public:
	GameManager();
	~GameManager();

	RECT GetWindowSize();

	void Init();
	void Update(float fSpeed);
	void Draw(HDC hdc);
	

};

