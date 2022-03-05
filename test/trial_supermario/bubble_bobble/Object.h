#pragma once
#include "framework.h"
#include "Screen.h"

class Object
{
protected:
	FPOINT pos;
	FPOINT vel;
	float moveSpeed;
	float jumpPower;
	bool IsOnGround;

	float gravity;
	int size;
	int life;


public:
	Object();
	~Object();

	FPOINT GetPos();
	FPOINT GetVec();

};

