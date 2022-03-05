#include "Enemy.h"

Enemy::Enemy(int x, int goaly, bool isRight)
{
	img = &Storage::GetInstance().GetEnemyImg(objState, isAttacking, frameMax);
	prevImg = img;

	isRight = isRight;

	// y축 0에서 goaly로 내려온다
	pos = { (float)x * TILE_SIZE, 0.0f };

	moveX = 0;
	moveY = 0;
	isMoveDone = true;
	mustJump = false;
	oldTime = GetTickCount();
	delayTime = DELAY_TIME_NOR;

	isAngry = false;
	isCaptured = false;
	readyToBeItem = false;
	notExists = false;
	reflectLeft = 0;

	SetVelWithGoal({ (float)x * TILE_SIZE, (float)goaly * TILE_SIZE });
}

Enemy::~Enemy() { }

bool Enemy::GetReadyToBeItem() { return readyToBeItem; }

bool Enemy::GetNotExists() { return notExists; }

bool Enemy::GetIsCaptured() { return isCaptured; }

void Enemy::SetNotExists(bool _notExists) { this->notExists = _notExists; }

void Enemy::SetAngry(bool _isAngry) 
{
	isAngry = _isAngry;
	if (isAngry == true)
	{
		moveSpeed = 1.5f;
		delayTime = DELAY_TIME_ANG;
	}
	else
	{
		moveSpeed = 1.0f;
		delayTime = DELAY_TIME_NOR;
	}
}

void Enemy::SetIsCaptured(bool _isCaptured)
{
	isCaptured = _isCaptured;
	isOnGround = false;
	SetPosWithTileNum(-2,-2);
}

void Enemy::SetReleased(fPoint bubblePos)
{
	isCaptured = false;
	SetPos(bubblePos);
}

void Enemy::SetDie(fPoint bubblePos, bool _isRight)
{
	objState = DEAD;
	isCaptured = false;
	isRight = _isRight;

	initPos = bubblePos;
	prevPos = bubblePos;

	if (isRight == true) deg = 180;
	else deg = 360;
}


// DEAD ---------------------------------------
bool Enemy::ReflectObject(Scene & s, float speed)
{
	fPoint nextPos = pos + vel * REFLECT_SPEED;
	
	// 바닥과 충돌하면 false리턴
	if (MoveWithGroundCollision(nextPos, s) == true)
	{
		pos = nextPos;
		vel.x = 0;
		MoveUpsideDown(pos);
		return false;
	}

	// 바닥과 충돌하지 않았다면 true리턴
	pos = nextPos;
	MoveUpsideDown(pos);
	return true;
}

void Enemy::FallObject(Scene & s, float speed)
{
	vel.x = 0.0f;
	vel.y = (GRAVITY * speed);

	fPoint nextPos = pos + vel;

	MoveWithGroundCollision(nextPos, s);
	MoveUpsideDown(nextPos);

	pos = nextPos;
}

bool Enemy::MoveWithHalfCircleAndReflection()
{
	if (deg < 180 || deg > 360 || reflectLeft != 0)
		return false;

	const float PI = 3.14159265358979;
	const int degree = 5; // 돌아가는 각도 크기 (속도)
	const int length = 4; // 돌아가는 거리량

	const int R = TILE_SIZE * length; // 큰 원의 반지름
	int X, Y; // 큰 원의 가운데 좌표

	int r = TILE_SIZE; // 플레이어의 반지름
	float rx, ry; // 돌아간 플레이어의 가운데 좌표

	if (isRight == true) X = initPos.x + R + TILE_SIZE * size; // 플레이어의 오른쪽 끝 + 큰 원의 반지름
	else                 X = initPos.x - R; // 플레이어의 왼쪽 끝 - 큰 원의 반지름

	Y = initPos.y + TILE_SIZE * size / 2;

	// 돌아가기
	rx = X + cos(deg * PI / 180) * (R + r);
	ry = Y + sin(deg * PI / 180) * (R + r);

	// 결과 반영
	pos.x = rx - TILE_SIZE * size / 2;
	pos.y = ry - TILE_SIZE * size / 2;

	if (ObjectVsSidewall(pos) == true)
	{
		deg = 0; // 반원 그리기 중지
		if (WALLX_LEFT * TILE_SIZE > pos.x) // 왼쪽에 닿았다
		{
			vel.x = 1;
			vel.y = 1;
		}
		else if (WALLX_RIGHT * TILE_SIZE < pos.x + TILE_SIZE * size) // 오른쪽에 닿았다
		{
			vel.x = -1;
			vel.y = 1;
		}

		pos = prevPos;
		reflectLeft = REFLECT_MAX;

		MoveUpsideDown(pos);
		return false;
	}

	// 다음 각도로 넘어가기
	if (isRight == true) deg += degree; // 오른쪽: 180 > 360
	else                 deg -= degree; // 왼쪽 : 360 > 180

	vel.x = (pos.x - prevPos.x) / std::abs(pos.x - prevPos.x);
	vel.y = (pos.y - prevPos.y) / std::abs(pos.y - prevPos.y);

	prevPos = pos;
	return true;
}

// AI ---------------------------------------
bool Enemy::IsJumpable(Scene & s, float posX)
{
	bool res;

	// size에 맞게 모든 타일이 위에 있을 때 true를 리턴한다
	for (int i = 1; i < 4; i++) // 최대 점프는 3칸 위까지
	{
		res = true;
		for (int j = 0; j < size + 1; j++)
		{
			// 상단 x변동, y고정
			// 위에 타일이 하나라도 없으면 false이다
			if (s.PosVsTile(posX + 0.9f * j * TILE_SIZE, pos.y - (i * TILE_SIZE)) == false) res = false;
		}
		if (res == true) return res;
	}

	return res;
}

int Enemy::GetCloseXDirToJump(Scene & s)
{
	int i = 0;

	// 현재 위치 점프 가능일 때
	if (IsJumpable(s, pos.x) == true) 	return 0;

	// 왼쪽이 점프 가능한지 살펴본다
	while (WALLX_LEFT * TILE_SIZE <= pos.x - i * TILE_SIZE)
	{
		if (IsJumpable(s, pos.x - i * TILE_SIZE) == true) return -1;

		i++;
	}

	i = 0;
	// 오른쪽이 점프 가능한지 살펴본다
	while (WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size >= pos.x + i * TILE_SIZE)
	{
		if (IsJumpable(s, pos.x + i * TILE_SIZE) == true) return 1;

		i++;
	}
	return 0;
}

bool Enemy::IsFallable(Scene & s, float posX)
{	
	for (int i = 0; i < size + 1; i++)
	{
		// 하단 x변동, y고정
		// 바닥이 하나라도 있으면 false
		if (s.PosVsTile(posX + (0.9f * i * TILE_SIZE), pos.y + TILE_SIZE * size) == true) return false;
	}
	// 바닥이 아예 없으면 떨어질 수 있으므르 true
	return true;
}

int Enemy::GetCloseXDirToFall(Scene & s)
{
	int i = 0, temp = i;

	// 현재 위치 떨어질 수 있을 때
	if (IsFallable(s, pos.x) == true) 	return 0;

	// 왼쪽과 오른쪽 중에 떨어질 수 있는 곳을 살펴본다.
	while (WALLX_LEFT * TILE_SIZE <= pos.x - i * TILE_SIZE && WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size >= pos.x + i * TILE_SIZE)
	{
		if (IsFallable(s, pos.x - i * TILE_SIZE) == true) return -1;
		if (IsFallable(s, pos.x + i * TILE_SIZE) == true) return 1;
		i++;
	}

	temp = i;

	// 둘 중 하나가 양 사이드 벽까지 떨어질 수 있는 곳이 없다면
	// 나머지 방향에 떨어질 수 있는 곳을 살펴본다.
	while (WALLX_LEFT * TILE_SIZE <= pos.x - i * TILE_SIZE)
	{
		if (IsFallable(s, pos.x - i * TILE_SIZE) == true) return -1;

		i++;
	}

	i = temp;
	while (WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size >= pos.x + i * TILE_SIZE)
	{
		if (IsFallable(s, pos.x + i * TILE_SIZE) == true) return 1;

		i++;
	}

	// 벽 바로 앞을 살펴본다
	if (IsFallable(s, WALLX_LEFT * TILE_SIZE) == true)                     return -1;
	if (IsFallable(s, WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size) == true) return 1;

	return 0;
}


void Enemy::MoveEnemy(fPoint playerPos, Scene & s, float speed)
{
	if (isCaptured == true)
		return;

	// 죽었을 때 움직임
	if (objState == DEAD)
	{
		// 반원 그리기 -> 벽 닿나 체크
		// 반원 x -> 벽 튕기는 중 -> 바닥 닿나 체크
		// 반원 x , 벽 x -> 바닥 닿나 체크
		if (MoveWithHalfCircleAndReflection() == false)
		{
			if (reflectLeft != 0)
			{
				if (ReflectObject(s, speed) == false) reflectLeft = 0;
				else                                     reflectLeft--;

			}
			else
			{
				FallObject(s, speed); // 떨어지기 -> 바닥에 닿으면 아이템이 된다
				if (isOnGround == true) readyToBeItem = true;
			}
		}
		return;
	}

	// 살았을 때 움직임
	newTime = GetTickCount();
	if (newTime - oldTime >= delayTime) // 딜레이 완료 시
	{
		oldTime = newTime;
		
		if (isMoveDone == true) // 행동 완료 시
		{
			isMoveDone = false;

			// 행동 결정하기
			if (playerPos.y < pos.y)
			{
				// 위에 있다면 가장 가까운 천장으로 올라가기
				mustJump = true;
				moveX = GetCloseXDirToJump(s);
			}
			else if (playerPos.y > pos.y)
			{
				// 아래있다면 가장 가까운 바닥으로 내려가기
				moveX = GetCloseXDirToFall(s);
			}
			else
			{
				// 같은 y축이라면
				if (playerPos.x < pos.x)      moveX = -1;
				else if (playerPos.x > pos.x) moveX = 1;
			}			
		}
	}

	if (objState == FALLING || objState == JUMPING)
	{
		isMoveDone = true;
		moveX = 0;
		moveY = 0;
	}

	if (mustJump == true && IsJumpable(s, pos.x) == true)
	{
		moveX = 0;
		moveY = 1;
		mustJump = false;
	}

	if (WALLX_LEFT * TILE_SIZE == pos.x || WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size == pos.x)
		isMoveDone = true;

	// 행동하기
	pos = MoveObject(moveX, moveY, s, speed);
}

void Enemy::DrawEnemy(Graphics * graphic)
{
	if (isCaptured == true || readyToBeItem == true) return;

	img = &Storage::GetInstance().GetEnemyImg(objState, isAngry, frameMax);

	DrawObject(graphic, 0, 0);
}

void Enemy::UpdateEnemyFrame()
{
	frameCounter++;

	if (ENEMY_DELAY_MAX != frameCounter) return;
	else if (ENEMY_DELAY_MAX == frameCounter) frameCounter = 0;

	frameCur++;
	if (frameCur >= frameMax) frameCur = frameMin;
}
