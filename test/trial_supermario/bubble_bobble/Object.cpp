#include "Object.h"

Object::Object()
{
	pos = { 0.0f, 0.0f };
	vel = { 0.0f, 0.0f };

	gravity = 0.2f;
	IsOnGround = false;
}

Object::~Object()
{
}

FPOINT Object::GetPos()
{
	return pos;
}

FPOINT Object::GetVec()
{
	return vel;
}
