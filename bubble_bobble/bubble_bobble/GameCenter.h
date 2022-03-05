#pragma once
#include "framework.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Input.h"
#include "Score.h"

class GameCenter
{
private:
	// Sound
	Sound * bgm;
	float bgmVol;

	const float EFFECT_VOL_DOWN = 0.2f;
	float effectVol;
	bool isEffectSoundOff;
	bool playJumpSound;

	// Screen
	SCREEN screen;
	SCREEN prevScreen;

	RECT windowSize;
	bool isGamePaused;
	bool canEnemyMove;
	bool isEnemyPaused_cheat;

	// Pause
	const int MENU_MAX = 5;
	int menu_cur;
	int isExitSelected;
	int selectedMenu;

	// Player
	DATA curUser;
	std::string name;
	int p1Score;
	int p2Score;
	bool isDataSaved;

	// Enemy 킬 스코어
	const int KILL_ONCE_TIMING = 15;
	int killOnceCnt;
	int killOnce;
	
	// hurry up
	bool drawHurryUp;
	bool isHurryUp;
	const DWORD HURRY_TIME = 10000;
	DWORD hurryOldTime;
	DWORD hurryNewTime;

	// hurry up 음악 재생 시 배경음 지연
	bool ReplayBgm;
	const DWORD BGM_TIME = 1000;
	DWORD bgmOldTime;
	DWORD bgmNewTime;

	// Round Change
	bool doRoundChange;
	const DWORD ROUND_CHANGE_TIME = 5000;
	DWORD roundChangeOldTime;
	DWORD roundChangeNewTime;

	Input * keyInput;
	Player * p;
	Enemy * e[7];
	Score * es;

	int enemyMax;
	int enemyCur;

	static GameCenter * instance;
	GameCenter();
	GameCenter(const GameCenter & ref);
	~GameCenter();

public:
	static GameCenter & GetInstance()
	{
		static GameCenter g;
		return g;
	}

	RECT GetWindowSize();
	bool GetIsExitSelected();

	void Init();

	void CreateEnemy();
	void RoundChange(int round);
	void InitHurryUp();

	void Update(float speed);
	void UpdateFrame();
	void Draw(HDC hdc);
};