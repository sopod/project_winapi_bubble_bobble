#include "Player.h"
#include "Draw.h"
#include "FPOINT.h"

Player::Player(int tileSize)
{
	size = 2;                                    // Ÿ�� ���� ����
	//pos = { 30,  40 };
	pos = { 3.0f * tileSize, 27.0f * tileSize};  // ������ġ 
	vel = { 0.0f, 0.0f };
	life = 3;
}

Player::~Player()
{
}

void Player::DrawObject(HDC hdc, int tileSize)
{
	DrawRectColor(hdc, { (int)pos.x, (int)pos.y }, size * tileSize, 255, 0, 255);
}
/*
void Player::MoveObject(float fElapsedTime, std::pair<int, int> rangeXPos, Map & m, int tileSize, int moveX, int moveY)
{	
	 vel = { 0, 0 };
	// vel.x = 0;

	// Ű������ ����
	if (moveX != 0)
	{
		vel.x = moveX * moveSpeed * fElapsedTime;
	}
	if (moveY != 0 && vel.y == 0)
	{
		IsJumping = true;
		vel.y += moveY * jumpPower;
	}
	if (vel.y >= 0)
	{
		IsJumping = false;
	}

	
	if (moveY != 0 && IsOnGround == true)
	{
		IsOnGround = false;
		jumpLeft = 4;
	}
	if (jumpLeft != 0)
	{
		IsJumping = true;
		vel.y += jumpPower;
		jumpLeft--;
	}
	else
	{
		IsJumping = false;
	}
	

	// �߷�
	vel.y += gravity * fElapsedTime;


	// ������ ���� �Ѿ�� (�¿�)
	if (rangeXPos.first > pos.x + vel.x || rangeXPos.second < pos.x + vel.x + tileSize * size)
		vel.x = 0;


	FPOINT nextPos = pos + vel * fElapsedTime;

	float mid = tileSize;
	float right_before_end = tileSize + tileSize * 0.9f;
	float end = tileSize * size;
	POINT chk;
	bool IsCollided = false;
	int res = 0;

	if (IsJumping == false)
	{
		// �������� �̵� ��
		if (vel.x <= 0)
		{
			chk = m.Collision(nextPos.x, pos.y, tileSize);
			if (chk.x != -1 && chk.y != -1)
			{
				res = chk.x;
				IsCollided = true;
			}
			else
			{
				chk = m.Collision(nextPos.x, pos.y + mid, tileSize);
				if (chk.x != -1 && chk.y != -1)
				{
					res = chk.x;
					IsCollided = true;
				}
				else
				{
					chk = m.Collision(nextPos.x, pos.y + right_before_end, tileSize);
					if (chk.x != -1 && chk.y != -1)
					{
						res = chk.x;
						IsCollided = true;
					}
				}
			}
			if (IsCollided == true)
			{
				nextPos.x = res * tileSize + tileSize;
				vel.x = 0;
			}
			
			POINT chk1 = m.Collision(nextPos.x, pos.y, tileSize);
			POINT chk2 = m.Collision(nextPos.x, pos.y + mid, tileSize);
			POINT chk3 = m.Collision(nextPos.x, pos.y + right_before_end, tileSize);
			if (chk1.x != 0 && chk1.y != 0)
			{
				nextPos.x = chk1.x * tileSize + tileSize;
				vel.x = 0;
			}
			else if (chk2.x != 0 && chk2.y != 0)
			{
				nextPos.x = chk2.x * tileSize + tileSize;
				vel.x = 0;
			}
			else if (chk3.x != 0 && chk3.y != 0)
			{
				nextPos.x = chk3.x * tileSize + tileSize;
				vel.x = 0;
			}
		}
		// ���������� �̵� ��
		else
		{
			chk = m.Collision(nextPos.x + end, pos.y, tileSize);
			if (chk.x != -1 && chk.y != -1)
			{
				res = chk.x;
				IsCollided = true;
			}
			else
			{
				chk = m.Collision(nextPos.x + end, pos.y + mid, tileSize);
				if (chk.x != -1 && chk.y != -1)
				{
					res = chk.x;
					IsCollided = true;
				}
				else
				{
					chk = m.Collision(nextPos.x + end, pos.y + right_before_end, tileSize);
					if (chk.x != -1 && chk.y != -1)
					{
						res = chk.x;
						IsCollided = true;
					}
				}
			}
			if (IsCollided == true)
			{
				nextPos.x = res * tileSize - tileSize * size;
				vel.x = 0;
			}
			
			POINT chk1 = m.Collision(nextPos.x + end, pos.y, tileSize);
			POINT chk2 = m.Collision(nextPos.x + end, pos.y + mid, tileSize);
			POINT chk3 = m.Collision(nextPos.x + end, pos.y + right_before_end, tileSize);
			if (chk1.x != 0 && chk1.y != 0)
			{
				nextPos.x = chk1.x * tileSize - tileSize * size;
				vel.x = 0;
			}
			else if (chk2.x != 0 && chk2.y != 0)
			{
				nextPos.x = chk2.x * tileSize - tileSize * size;
				vel.x = 0;
			}
			else if (chk3.x != 0 && chk3.y != 0)
			{
				nextPos.x = chk3.x * tileSize - tileSize * size;
				vel.x = 0;
			}
		}

		IsCollided = false;

		// ���� �̵� ��
		if (vel.y <= 0)
		{
			chk = m.Collision(nextPos.x, nextPos.y, tileSize);
			if (chk.x != -1 && chk.y != -1)
			{
				res = chk.y;
				IsCollided = true;
			}
			else
			{
				chk = m.Collision(nextPos.x + mid, nextPos.y, tileSize);
				if (chk.x != -1 && chk.y != -1)
				{
					res = chk.y;
					IsCollided = true;
				}
				else
				{
					chk = m.Collision(nextPos.x + right_before_end, nextPos.y, tileSize);
					if (chk.x != -1 && chk.y != -1)
					{
						res = chk.y;
						IsCollided = true;
					}
				}
			}
			if (IsCollided == true)
			{
				nextPos.y = res * tileSize + tileSize;
				vel.y = 0;
			}
			
			POINT chk1 = m.Collision(nextPos.x, nextPos.y, tileSize);
			POINT chk2 = m.Collision(nextPos.x + mid, nextPos.y, tileSize);
			POINT chk3 = m.Collision(nextPos.x + right_before_end, nextPos.y, tileSize);
			if (chk1.x != 0 && chk1.y != 0)
			{
				nextPos.y = chk1.y * tileSize + tileSize;
				vel.y = 0;
			}
			else if (chk2.x != 0 && chk2.y != 0)
			{
				nextPos.y = chk2.y * tileSize + tileSize;
				vel.y = 0;
			}
			else if (chk3.x != 0 && chk3.y != 0)
			{
				nextPos.y = chk3.y * tileSize  + tileSize;
				vel.y = 0;
			}
		}
		// �Ʒ��� �̵� ��
		else
		{
			chk = m.Collision(nextPos.x, nextPos.y + end, tileSize);
			if (chk.x != -1 && chk.y != -1)
			{
				res = chk.y;
				IsCollided = true;
			}
			else
			{
				chk = m.Collision(nextPos.x + mid, nextPos.y + end, tileSize);
				if (chk.x != -1 && chk.y != -1)
				{
					res = chk.y;
					IsCollided = true;
				}
				else
				{
					chk = m.Collision(nextPos.x + right_before_end, nextPos.y + end, tileSize);
					if (chk.x != -1 && chk.y != -1)
					{
						res = chk.y;
						IsCollided = true;
					}
				}
			}
			if (IsCollided == true)
			{
				nextPos.y = res * tileSize - tileSize * size;
				vel.y = 0;
				IsOnGround = true;
			}

			
			POINT chk1 = m.Collision(nextPos.x, nextPos.y + end, tileSize);
			POINT chk2 = m.Collision(nextPos.x + mid, nextPos.y + end, tileSize);
			POINT chk3 = m.Collision(nextPos.x + right_before_end, nextPos.y + end, tileSize);
			if (chk1.x != 0 && chk1.y != 0)
			{
				nextPos.y = chk1.y * tileSize - tileSize * size;
				vel.y = 0;
				IsOnGround = true;
			}
			else if (chk2.x != 0 && chk2.y != 0)
			{
				nextPos.y = chk2.y * tileSize - tileSize * size;
				vel.y = 0;
				IsOnGround = true;
			}
			else if (chk3.x != 0 && chk3.y != 0)
			{
				nextPos.y = chk3.y * tileSize - tileSize * size;
				vel.y = 0;
				IsOnGround = true;
			}
		}
	}	

	pos = nextPos;
}
*/

void Player::MoveObject(float fElapsedTime, std::pair<int, int> rangeXPos, std::vector<FPOINT> & platforms, int tileSize, int moveX, int moveY)
{
	FPOINT cp, cn;
	float ct;
	static std::vector<std::pair<FPOINT, float>> z;

	//vel = { 0, 0 };
	vel.x = 0;
	//fElapsedTime = 1;

	

	// Ű������ ����
	if (moveX != 0)
	{
		vel.x += moveX * moveSpeed * fElapsedTime;
	}
	if (moveY != 0 && vel.y == 0)
	{
		IsJumping = true;
		vel.y += moveY * jumpPower;
	}
	if (vel.y >= 0)
	{
		IsJumping = false;
	}
	/*
	if (moveY != 0 && IsOnGround == true)
	{
		IsOnGround = false;
		jumpLeft = 4;
	}
	if (jumpLeft != 0)
	{
		IsJumping = true;
		vel.y += jumpPower * fElapsedTime;
		jumpLeft--;
	}
	else
	{
		IsJumping = false;
	}*/

	// �߷�
	vel.y += gravity * fElapsedTime;

	if (vel.y > 1.0f)
		vel.y = 1.0f;
	else if (vel.y < -15.0f)
		vel.y = -15.0f;


	// ������ ���� �Ѿ�� (�¿�)
	if (rangeXPos.first > pos.x + vel.x || rangeXPos.second < pos.x + vel.x + tileSize * size)
		vel.x = 0;

	// �÷����� �浹 �ϸ� �÷��� ���� �ø���
	IsOnGround = false;
	for (FPOINT i : platforms)
	{
		//if (RectVsRect(pos, tileSize * size, i, tileSize))
		if (DynamicRectVsRect(*this, i, tileSize, cp, cn, ct, fElapsedTime))
		{
			if (IsJumping == false) // ���� �� �浹 �� ��
			{
				IsOnGround = true;
				z.push_back({ i, (float)ct });
			}
		}
	}

	std::sort(z.begin(), z.end(), [](const std::pair<FPOINT, float>& a, const std::pair<FPOINT, float> & b)
	{
		return a.second < b.second;
	});

	for (auto j : z)
		if (DynamicRectVsRect(*this, j.first, tileSize, cp, cn, ct, fElapsedTime))
		{
			vel += vel.Abs() * (1 - ct) * cn;
			// �浹 ���������� velocity�� ����Ѵ�.
			// pos --> next pos�� time�� �� 1�̴ϱ�
			// contact time --> next pos������ 1 - contact time�̴�. vel * (1 - ct)
			// ���⿡ normal ���͸� ���� �־ ������ �ݴ�� �����. contact time <-- next pos
		}

	z.clear();
	pos += vel * fElapsedTime;
}

void Player::DeleteObject()
{
}




