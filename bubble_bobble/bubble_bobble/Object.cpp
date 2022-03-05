#include "Object.h"
#include "Node.h"
#include <queue>

Object::Object()
{
	pos = { 0.0f, 0.0f };
	size = 2;
	life = 1;

	moveSpeed = 1.0f;
	jumpCur = JUMP_MAX;

	InitObjectAsGameState({ 0.0f, 0.0f });
}

Object::~Object() { }

void Object::InitObjectAsGameState(fPoint _pos)
{
	// 게임 시작 상태로 되돌리기
	objState = IDLE;
	pos = _pos;
	vel = { 0.0f, 0.0f }; 

	isRight = true;
	isOnGround = true;
	isJumping = false;
	isAttacking = false;
	isInvincible = false;
	isMoveToGoalDone = false;

	oldTime = GetTickCount();
	frameCounter = 0;
	frameMin = 0;
	frameCur = 0;
}

STATE Object::GetObjectState() { return objState; }

fPoint Object::GetPos() { return pos; }

fPoint Object::GetVec() { return vel; }

int Object::GetSize() { return size; }

int Object::GetLife() { return life; }

bool Object::GetIsRight() { return isRight; }

bool Object::GetIsOnGround() { return isOnGround; }

bool Object::GetIsInvincible() { return isInvincible; }

bool Object::GetIsMoveToGoalDone() { return isMoveToGoalDone; }

void Object::SetLife(int _life) { life = _life;}

void Object::SetObjectState(STATE _state) { objState = _state; }

void Object::SetIsRight(bool _IsRight) { isRight = _IsRight; }

void Object::SetIsOnGround(bool _IsOnGround) { isOnGround = _IsOnGround; }

void Object::SetPos(fPoint _pos) { pos = { _pos.x, _pos.y }; }

void Object::SetPosWithTileNum(int x, int y)
{
	pos = { (float)x * TILE_SIZE, (float)y * TILE_SIZE };
}

void Object::SetVel(float x, float y)
{
	vel.x = x;
	vel.y = y;
}

void Object::SetVelWithGoal(fPoint goal)
{
	goalPos = goal;

	vel = { 0.0f, 0.0f };
	
	float div = sqrt(pow((goal.x - pos.x), 2) + pow((goal.y - pos.y), 2));

	if (pos.x != goal.x) vel.x = (goal.x - pos.x) / div;

	if (pos.y != goal.y) vel.y = (goal.y - pos.y) / div;

	if (vel.x == 0 && vel.y == 0) isMoveToGoalDone = true;
}

void Object::VelControl()
{
	// 타일을 넘어서는 힘은 안 된다.
	if (std::abs(vel.x) >= TILE_SIZE)
	{
		if (vel.x < 0) vel.x = -(TILE_SIZE - 1);
		else           vel.x = (TILE_SIZE - 1);
	}
	if (std::abs(vel.y) >= TILE_SIZE)
	{
		if (vel.y < 0) vel.y = -(TILE_SIZE - 1);
		else           vel.y = (TILE_SIZE - 1);
	}
}

void Object::MoveToGoal(float fSpeed, bool DoesRangeXMatter)
{
	if (isMoveToGoalDone == true) return;

	// 더이상 움직일 곳이 없다 = 골에 도착했다면 
	if (vel.x == 0 && vel.y == 0) isMoveToGoalDone = true;

	// 타일을 넘어서는 힘은 안 된다
	VelControl();

	// x축 이동시 양쪽 벽은 통과 못하게 하기
	if (DoesRangeXMatter == true && ObjectVsSidewall({ pos.x + (vel.x * fSpeed), pos.y }) == true)
		vel.x = 0;

	// fSpeed로 인해 생기는 움직임량 보정
	if (vel.y > 0 && pos.y + vel.y * fSpeed > goalPos.y)
	{
		pos.y = goalPos.y;
		vel.y = 0;
	}
	else if (vel.y < 0 && pos.y + vel.y * fSpeed < goalPos.y)
	{
		pos.y = goalPos.y;
		vel.y = 0;
	}

	if (vel.x > 0 && pos.x + vel.x * fSpeed > goalPos.x)
	{
		pos.x = goalPos.x;
		vel.x = 0;
	}
	else if (vel.x < 0 && pos.x + vel.x * fSpeed < goalPos.x)
	{
		pos.x = goalPos.x;
		vel.x = 0;
	}
	
	// 이동 시행
	pos += (vel * fSpeed);
}



fPoint Object::MoveObject(int moveX, int jumpKey, Scene & s, float fSpeed)
{
	vel = { 0.0f, 0.0f };

	// 점프 키가 눌렸고, 땅에 서있으면 점프한다
	if (jumpKey != 0 && isOnGround)
	{
		jumpCur = 0;
		isOnGround = false;
		isJumping = true;
		objState = JUMPING;
	}

	if (jumpCur > JUMP_MAX)
	{
		if (JUMP_POWER * fSpeed < JUMP_MAX - jumpCur) vel.y += (JUMP_MAX - jumpCur);
		else                                          vel.y += (JUMP_POWER * fSpeed);
		jumpCur += vel.y;
	}
	else
	{
		isJumping = false;
	}

	if (!isJumping && !isOnGround)     objState = FALLING;
	else if (!isJumping && isOnGround) objState = (moveX != 0) ? WALKING : IDLE;

	// 중력 적용
	vel.y += (GRAVITY * fSpeed);
	
	// 좌우 이동
	vel.x = moveX * moveSpeed * fSpeed;

	if (moveX < 0)      isRight = false;
	else if (moveX > 0)	isRight = true;

	// 타일 충돌
	fPoint nextPos = MoveWithTileCollision(pos, s);
	MoveUpsideDown(nextPos);
	return nextPos;
}

fPoint Object::MoveWithTileCollision(fPoint & pos, Scene & s)
{
	fPoint zero = { 0.0f, 0.0f };
	fPoint cur = { 0.0f, 0.0f };
 
	VelControl();

	fPoint nextPos = pos + vel;

	// 점프 중에는 좌우 타일만 신경쓴다
	if (isJumping == true)
	{
		if (ObjectVsSidewall(nextPos) == true) vel.x = 0;
		return pos + vel;
	}

	// 좌우, 상하를 나눠서 살펴본다 
	if (vel.x <= 0) // LEFT
	{
		for (int i = 0; i < size + 1; i++)
		{
			// 좌측의 고정x, 변동y
			cur = s.PosVsTileWhere(nextPos.x, pos.y + (0.9f * i * TILE_SIZE));
			if (cur != zero)
			{
				nextPos.x = cur.x + TILE_SIZE;
				vel.x = 0;
				break;
			}
		}
	}
	else if (vel.x > 0) // RIGHT
	{
		for (int i = 0; i < size + 1; i++)
		{
			// 우측의 고정x, 변동y
			cur = s.PosVsTileWhere(nextPos.x + (size * TILE_SIZE), pos.y + (0.9f * i * TILE_SIZE));
			if (cur != zero)
			{
				nextPos.x = cur.x - (size * TILE_SIZE);
				vel.x = 0;
				break;
			}
		}
	}
	

	isOnGround = false;

	if (vel.y <= 0) // UP
	{
		for (int i = 0; i < size + 1; i++)
		{
			// 상단의 변동x, 고정y
			cur = s.PosVsTileWhere(nextPos.x + (0.9f * i * TILE_SIZE), nextPos.y);
			if (cur != zero)
			{
				nextPos.y = cur.y + TILE_SIZE;
				vel.y = 0;
				break;
			}
		}
	}
	else if (vel.y > 0) // DOWN
	{
		for (int i = 0; i < size + 1; i++)
		{
			// 하단의 변동x, 고정y
			cur = s.PosVsTileWhere(nextPos.x + (0.9f * i * TILE_SIZE), nextPos.y + (size * TILE_SIZE));
			if (cur != zero)
			{
				nextPos.y = cur.y - (size * TILE_SIZE);
				vel.y = 0;
				isOnGround = true;
				isJumping = false;
				break;
			}
		}
	}
	
	return nextPos;
}

void Object::MoveUpsideDown(fPoint & nextPos)
{
	if (pos.y >= MAP_HEIGHT * TILE_SIZE)
		nextPos.y = pos.y - ((MAP_HEIGHT - 2) * TILE_SIZE);
}

bool Object::MoveWithGroundCollision(fPoint & nextPos, Scene & s)
{
	fPoint zero = { 0.0f, 0.0f };
	fPoint cur = { 0.0f, 0.0f };

	isOnGround = false;

	for (int i = 0; i < size + 1; i++)
	{
		cur = s.PosVsTileWhere(nextPos.x + (0.9f * i * TILE_SIZE), nextPos.y + (size * TILE_SIZE));
		if (cur != zero)
		{
			nextPos.y = cur.y - (size * TILE_SIZE);
			vel.y = 0;
			isOnGround = true;
			isJumping = false;
			return true;
		}
	}
	return false;
}

bool Object::ObjectVsObject(Object & o)
{
	return (pos.x < o.pos.x + o.size * TILE_SIZE && pos.x + size * TILE_SIZE > o.pos.x &&
		pos.y < o.pos.y + o.size * TILE_SIZE && pos.y + size * TILE_SIZE > o.pos.y);
}

bool Object::ObjectVsSidewall(fPoint pos)
{
	return (pos.x < WALLX_LEFT* TILE_SIZE || WALLX_RIGHT * TILE_SIZE < pos.x + TILE_SIZE * size);
}

void Object::DrawObject(Graphics * graphic, float gapX, float gapY)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(15, 79, 174), Color(15, 79, 174));

	if (isInvincible)
	{
		static REAL transparency = 0.5f; // 투명도
		transparency += 0.1f;            // 투명도 지속적으로 조절
		if (transparency > 1.0f) transparency = 0.5f;

		ColorMatrix colorMatrix =
		{
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, transparency, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		imgAttr.SetColorMatrix(&colorMatrix);
	}
	else
	{
		ColorMatrix colorMatrix =
		{
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		imgAttr.SetColorMatrix(&colorMatrix);
	}

	if (img[0] != prevImg[0]) // 스프라이트가 바뀌었다면, 
	{
		prevImg = img;
		frameCur = frameMin;
		frameCounter = 0;
	}

	int width = (*img)[frameCur]->GetWidth();
	int height = (*img)[frameCur]->GetHeight();

	POINT spos;
	Rect dest;

	if (isRight == false)
	{
		// 오브젝트의 우하단을 기준으로 이미지 크기만큼 빼서 오브젝트의 좌상단 위치를 구한다.
		spos = { (int)pos.x + size * TILE_SIZE - width * 2, (int)pos.y + size * TILE_SIZE - height * 2 };
		dest = { spos.x + (int)gapX, spos.y + (int)gapY, width * 2, height * 2 };  //  화면 위치, 화면 크기
	}
	else
	{
		// 우측은 -width를 하고, 위치를 타일 크기만큼 x축에 더해준다
		spos = { (int)pos.x + 2 * size * TILE_SIZE - width * 2, (int)pos.y + size * TILE_SIZE - height * 2 };
		dest = { spos.x + (int)gapX * 3, spos.y + (int)gapY, -width * 2, height * 2 };
	}
		
	graphic->DrawImage((*img)[frameCur], dest, 0, 0, width, height,  // 이미지 위치, 이미지 크기
		UnitPixel, &imgAttr);
}
