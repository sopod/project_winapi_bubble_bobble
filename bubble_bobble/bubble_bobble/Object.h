#pragma once
#include "framework.h"
#include "Scene.h"

class Object
{
protected:
	std::vector<Image *> * img;
	std::vector<Image *> * prevImg;

	STATE objState;

	fPoint pos;
	fPoint vel;
	int size;
	int life;
	
	fPoint goalPos;
	fPoint prevPos;

	float moveSpeed;
	float jumpCur;

	bool isRight;
	bool isOnGround;
	bool isJumping;
	bool isAttacking;
	bool isInvincible;
	bool isMoveToGoalDone;	

	DWORD oldTime;
	DWORD newTime;

	int frameCounter;
	int frameMax;
	int frameMin;
	int frameCur;

public:
	Object();
	~Object();

	void InitObjectAsGameState(fPoint pos);

	STATE GetObjectState();
	fPoint GetPos();
	fPoint GetVec();
	int GetSize();
	int GetLife();
	bool GetIsRight();
	bool GetIsOnGround();
	bool GetIsInvincible();
	bool GetIsMoveToGoalDone();

	void SetLife(int life);
	void SetObjectState(STATE state);
	void SetIsRight(bool isRight);
	void SetIsOnGround(bool isOnGround);

	void SetPos(fPoint pos);
	void SetPosWithTileNum(int x, int y);
	void SetVel(float x, float y);
	void SetVelWithGoal(fPoint goal);

	void VelControl();
	void MoveToGoal(float speed, bool doesRangeXMatter);
	fPoint MoveObject(int moveX, int jumpKey, Scene & s, float speed);
	fPoint MoveWithTileCollision(fPoint & pos, Scene & s);
	void MoveUpsideDown(fPoint & nextPos);
	bool MoveWithGroundCollision(fPoint & pos, Scene & s);

	bool ObjectVsObject(Object & o);
	bool ObjectVsSidewall(fPoint pos);

	void DrawObject(Graphics * graphic, float gapX, float gapY);
	
};
