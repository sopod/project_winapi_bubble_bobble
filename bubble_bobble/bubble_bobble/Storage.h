#pragma once
#include "framework.h"
#include "Sound.h"

// 전역 변수
enum SCREEN
{
	LOBBY, GAME, ROUNDCHANGE, SCORE, GAMEOVER
};

enum STATE
{
	IDLE, WALKING, JUMPING, FALLING, DEAD
};

enum B_STATE
{
	SLIDING, NORMAL, CORROSION1, CORROSION2, POP
};

enum B_TYPE
{
	DEFAULT, WATER
};

enum COLOR
{
	BLUE, GREEN, RED, WHITE, YELLOW
};

enum PLAYER_SOUND
{
	JUMP, ATTACK, POPBUBBLE, GETITEM, GOODBYE
};

struct DATA
{
	int score = 0;
	int round = 0;
	char name[4] = "";
};

// tile, window
const int TILE_SIZE = 16;
const int MAP_WIDTH = 32;   // 타일 가로 개수
const int MAP_HEIGHT = 28;  // 타일 세로 개수
const int WALLX_LEFT = 2;  // 최소 x축 이동 좌표
const int WALLX_RIGHT = MAP_WIDTH - 2; // 최대 x축 이동 좌표
const int WALLY = 3;

// Init
const int START_ROUND = 1;
const int MAX_ROUND = 3;

// Object
const float GRAVITY = 1.0f;
const float JUMP_POWER = -3.0f;
const float JUMP_MAX = -135.0f; // 점프 최대치

// Player
const fPoint START_POS = { 4.0f, 25.0f }; 
const fPoint CENTER_POS = { (float)((MAP_WIDTH * TILE_SIZE) / 2 - TILE_SIZE * 2 / 2), (float)(((MAP_HEIGHT + 2) * TILE_SIZE) / 2 - TILE_SIZE * 2 / 2) };
const int START_LIFE = 3;
const int BUBBLE_COLLISION_GAP_LEFT = 15;
const int BUBBLE_COLLISION_GAP_RIGHT = 10;
const int BUBBLE_COLLISION_JUMP_GAP = TILE_SIZE;

const int PLAYER_DELAY_MAX = 10;
const DWORD INVINCIBLE_TIME = 3000;
const int MOTION_REPEAT_MAX = 7;

// Enemy
const int REFLECT_SPEED = 4;
const int REFLECT_MAX = 20;

const int ENEMY_DELAY_MAX = 10;
const int DELAY_TIME_NOR = 150;
const int DELAY_TIME_ANG = 70;

// Bubble
const fPoint BUBBLE_GOAL = { 15 * TILE_SIZE, 4 * TILE_SIZE };
const int BUBBLE_DELAY_MAX = 5;

const int SLIDE_MOVEMENT = 5 * TILE_SIZE;
const int TOP_MOVEMENT = 8 * TILE_SIZE;

const DWORD CORROSION1_TIME = 10000;
const DWORD CORROSION2_TIME = 15000;
const DWORD POP_TIME = 17000;

// Item
const int ITME_WIDTH = 16;
const int SCORE_MOVEMENT = TILE_SIZE * 3;

// Score 	
const int IMG_DATA[4][2] = { {0, 0}, {8, 10}, {3, 15}, {2,16} }; // gap, width



class Storage
{
private:
	// Image
	std::vector<wchar_t *> f [50];
	
	std::vector<Image *> player[10];
	std::vector<Image *> enemy[7];
	std::vector<Image *> tile[1];
	std::vector<Image *> character[1];
	std::vector<Image *> bubble[5];
	std::vector<Image *> item[1];

	// Score
	DATA top[5];

	// Sound
	std::vector<char *> b [10];
	std::vector<Sound *> bgmOnce;
	std::vector<Sound *> bgmIng;
	std::vector<Sound *> bgmEffect;
	std::vector<Sound *> playerSound;

	static Storage * instance;
	Storage();
	Storage(const Storage & ref);
	~Storage();

public:
	static Storage & GetInstance()
	{
		static Storage s;
		return s;
	}

	void FileInit();

	// Image
	void LoadImageFile();	
	wchar_t * CharToWideChar(char * str);
	void DestroyImageFile(std::vector<Image *> *img);

	std::vector<Image *> & GetPlayerImg(STATE state, bool isAttacking, bool isStartMotion, int & frameMax);
	std::vector<Image *> & GetEnemyImg(STATE state, bool isAngry, int & frameMax);
	std::vector<Image *> & GetTileImg();
	std::vector<Image *> & GetCharImg();
	std::vector<Image *> & GetBubbleImg(B_STATE state, bool isContainEnemy, int & frameMax);
	std::vector<Image *> & GetItemImg();

	// Score
	void SaveTopFiveDataFile(DATA & curUser);
	void LoadDataFile();
	DATA * GetTop();
	int GetTopScore();

	// Sound
	void LoadSoundFile();	
	Sound * & GetBgm(SCREEN screen, bool isHurryUp);
	Sound * & GetBgmEffect();
	Sound * & GetPlayerSound(PLAYER_SOUND ps);
	void DestroySoundFile(std::vector<Sound *> & sound);	
};

