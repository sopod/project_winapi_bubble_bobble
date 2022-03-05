#pragma once
#include "framework.h"
#include "Map.h"

class Object
{
protected:
	FPOINT pos;
	FPOINT vel;

	float gravity;
	float jumpPower;
	float moveSpeed;

	bool IsOnGround;
	bool IsJumping;
	int jumpLeft;
	

	int size;
	int life;

public:
	Object();
	~Object();

	bool RectVsRect(FPOINT & r1, int size1, FPOINT & r2, int size2);

	bool RayVsRect(FPOINT & ray_origin, FPOINT & ray_dir, FPOINT & target, int targetSize, 
		FPOINT & contact_point, FPOINT & contact_normal, float & t_hit_near);

	bool DynamicRectVsRect(Object& in, FPOINT& target, int& TargetSize,
		FPOINT & contact_point, FPOINT & contact_normal, float & contact_time, float fElapsedTime);
};

