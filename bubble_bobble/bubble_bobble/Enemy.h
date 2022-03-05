#pragma once
#include "Object.h"
#include "Player.h"

class Enemy : public Object
{
private:
	fPoint initPos;
	int moveX;
	int moveY;

	int deg;
	int reflectLeft;

	bool isMoveDone;
	bool mustJump;
	int delayTime;

	bool isAngry;
	bool isCaptured;
	bool readyToBeItem;
	bool notExists;

public:
	Enemy(int x, int y, bool isRight);
	~Enemy();

	bool GetIsCaptured();
	bool GetReadyToBeItem();
	bool GetNotExists();

	void SetAngry(bool isAngry);
	void SetIsCaptured(bool isCaptured);
	void SetNotExists(bool _notExists);

	void SetReleased(fPoint bubblePos);
	void SetDie(fPoint bubblePos, bool isRight);

	// -------------- DEAD
	bool ReflectObject(Scene & s, float speed);
	void FallObject(Scene & s, float speed);
	bool MoveWithHalfCircleAndReflection();

	// -------------- AI
	bool IsJumpable(Scene & s, float posY);
	int GetCloseXDirToJump(Scene & s);
	bool IsFallable(Scene & s, float posX);
	int GetCloseXDirToFall(Scene & s);

	void MoveEnemy(fPoint playerPos, Scene & s, float speed);
	
	void DrawEnemy(Graphics *graphic);
	void UpdateEnemyFrame();

};