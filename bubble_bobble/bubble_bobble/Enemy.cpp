#include "Enemy.h"

Enemy::Enemy(int x, int goaly, bool isRight)
{
	img = &Storage::GetInstance().GetEnemyImg(objState, isAttacking, frameMax);
	prevImg = img;

	isRight = isRight;

	// y�� 0���� goaly�� �����´�
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
	
	// �ٴڰ� �浹�ϸ� false����
	if (MoveWithGroundCollision(nextPos, s) == true)
	{
		pos = nextPos;
		vel.x = 0;
		MoveUpsideDown(pos);
		return false;
	}

	// �ٴڰ� �浹���� �ʾҴٸ� true����
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
	const int degree = 5; // ���ư��� ���� ũ�� (�ӵ�)
	const int length = 4; // ���ư��� �Ÿ���

	const int R = TILE_SIZE * length; // ū ���� ������
	int X, Y; // ū ���� ��� ��ǥ

	int r = TILE_SIZE; // �÷��̾��� ������
	float rx, ry; // ���ư� �÷��̾��� ��� ��ǥ

	if (isRight == true) X = initPos.x + R + TILE_SIZE * size; // �÷��̾��� ������ �� + ū ���� ������
	else                 X = initPos.x - R; // �÷��̾��� ���� �� - ū ���� ������

	Y = initPos.y + TILE_SIZE * size / 2;

	// ���ư���
	rx = X + cos(deg * PI / 180) * (R + r);
	ry = Y + sin(deg * PI / 180) * (R + r);

	// ��� �ݿ�
	pos.x = rx - TILE_SIZE * size / 2;
	pos.y = ry - TILE_SIZE * size / 2;

	if (ObjectVsSidewall(pos) == true)
	{
		deg = 0; // �ݿ� �׸��� ����
		if (WALLX_LEFT * TILE_SIZE > pos.x) // ���ʿ� ��Ҵ�
		{
			vel.x = 1;
			vel.y = 1;
		}
		else if (WALLX_RIGHT * TILE_SIZE < pos.x + TILE_SIZE * size) // �����ʿ� ��Ҵ�
		{
			vel.x = -1;
			vel.y = 1;
		}

		pos = prevPos;
		reflectLeft = REFLECT_MAX;

		MoveUpsideDown(pos);
		return false;
	}

	// ���� ������ �Ѿ��
	if (isRight == true) deg += degree; // ������: 180 > 360
	else                 deg -= degree; // ���� : 360 > 180

	vel.x = (pos.x - prevPos.x) / std::abs(pos.x - prevPos.x);
	vel.y = (pos.y - prevPos.y) / std::abs(pos.y - prevPos.y);

	prevPos = pos;
	return true;
}

// AI ---------------------------------------
bool Enemy::IsJumpable(Scene & s, float posX)
{
	bool res;

	// size�� �°� ��� Ÿ���� ���� ���� �� true�� �����Ѵ�
	for (int i = 1; i < 4; i++) // �ִ� ������ 3ĭ ������
	{
		res = true;
		for (int j = 0; j < size + 1; j++)
		{
			// ��� x����, y����
			// ���� Ÿ���� �ϳ��� ������ false�̴�
			if (s.PosVsTile(posX + 0.9f * j * TILE_SIZE, pos.y - (i * TILE_SIZE)) == false) res = false;
		}
		if (res == true) return res;
	}

	return res;
}

int Enemy::GetCloseXDirToJump(Scene & s)
{
	int i = 0;

	// ���� ��ġ ���� ������ ��
	if (IsJumpable(s, pos.x) == true) 	return 0;

	// ������ ���� �������� ���캻��
	while (WALLX_LEFT * TILE_SIZE <= pos.x - i * TILE_SIZE)
	{
		if (IsJumpable(s, pos.x - i * TILE_SIZE) == true) return -1;

		i++;
	}

	i = 0;
	// �������� ���� �������� ���캻��
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
		// �ϴ� x����, y����
		// �ٴ��� �ϳ��� ������ false
		if (s.PosVsTile(posX + (0.9f * i * TILE_SIZE), pos.y + TILE_SIZE * size) == true) return false;
	}
	// �ٴ��� �ƿ� ������ ������ �� �����Ǹ� true
	return true;
}

int Enemy::GetCloseXDirToFall(Scene & s)
{
	int i = 0, temp = i;

	// ���� ��ġ ������ �� ���� ��
	if (IsFallable(s, pos.x) == true) 	return 0;

	// ���ʰ� ������ �߿� ������ �� �ִ� ���� ���캻��.
	while (WALLX_LEFT * TILE_SIZE <= pos.x - i * TILE_SIZE && WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size >= pos.x + i * TILE_SIZE)
	{
		if (IsFallable(s, pos.x - i * TILE_SIZE) == true) return -1;
		if (IsFallable(s, pos.x + i * TILE_SIZE) == true) return 1;
		i++;
	}

	temp = i;

	// �� �� �ϳ��� �� ���̵� ������ ������ �� �ִ� ���� ���ٸ�
	// ������ ���⿡ ������ �� �ִ� ���� ���캻��.
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

	// �� �ٷ� ���� ���캻��
	if (IsFallable(s, WALLX_LEFT * TILE_SIZE) == true)                     return -1;
	if (IsFallable(s, WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size) == true) return 1;

	return 0;
}


void Enemy::MoveEnemy(fPoint playerPos, Scene & s, float speed)
{
	if (isCaptured == true)
		return;

	// �׾��� �� ������
	if (objState == DEAD)
	{
		// �ݿ� �׸��� -> �� �곪 üũ
		// �ݿ� x -> �� ƨ��� �� -> �ٴ� �곪 üũ
		// �ݿ� x , �� x -> �ٴ� �곪 üũ
		if (MoveWithHalfCircleAndReflection() == false)
		{
			if (reflectLeft != 0)
			{
				if (ReflectObject(s, speed) == false) reflectLeft = 0;
				else                                     reflectLeft--;

			}
			else
			{
				FallObject(s, speed); // �������� -> �ٴڿ� ������ �������� �ȴ�
				if (isOnGround == true) readyToBeItem = true;
			}
		}
		return;
	}

	// ����� �� ������
	newTime = GetTickCount();
	if (newTime - oldTime >= delayTime) // ������ �Ϸ� ��
	{
		oldTime = newTime;
		
		if (isMoveDone == true) // �ൿ �Ϸ� ��
		{
			isMoveDone = false;

			// �ൿ �����ϱ�
			if (playerPos.y < pos.y)
			{
				// ���� �ִٸ� ���� ����� õ������ �ö󰡱�
				mustJump = true;
				moveX = GetCloseXDirToJump(s);
			}
			else if (playerPos.y > pos.y)
			{
				// �Ʒ��ִٸ� ���� ����� �ٴ����� ��������
				moveX = GetCloseXDirToFall(s);
			}
			else
			{
				// ���� y���̶��
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

	// �ൿ�ϱ�
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
