#include "Player.h"
#include "Draw.h"
#include "FPOINT.h"

Player::Player(int tileSize)
{
	size = 2;                                    // 타일 가로 개수
	//pos = { 30,  40 };
	pos = { 3.0f * tileSize, 27.0f * tileSize};  // 시작위치 
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

	// 키보드의 조작
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
	

	// 중력
	vel.y += gravity * fElapsedTime;


	// 윈도우 밖을 넘어서면 (좌우)
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
		// 왼쪽으로 이동 시
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
		// 오른쪽으로 이동 시
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

		// 위로 이동 시
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
		// 아래로 이동 시
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

	

	// 키보드의 조작
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

	// 중력
	vel.y += gravity * fElapsedTime;

	if (vel.y > 1.0f)
		vel.y = 1.0f;
	else if (vel.y < -15.0f)
		vel.y = -15.0f;


	// 윈도우 밖을 넘어서면 (좌우)
	if (rangeXPos.first > pos.x + vel.x || rangeXPos.second < pos.x + vel.x + tileSize * size)
		vel.x = 0;

	// 플랫폼과 충돌 하면 플랫폼 위로 올리기
	IsOnGround = false;
	for (FPOINT i : platforms)
	{
		//if (RectVsRect(pos, tileSize * size, i, tileSize))
		if (DynamicRectVsRect(*this, i, tileSize, cp, cn, ct, fElapsedTime))
		{
			if (IsJumping == false) // 점프 시 충돌 안 함
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
			// 충돌 직전까지의 velocity를 계산한다.
			// pos --> next pos의 time이 늘 1이니까
			// contact time --> next pos까지는 1 - contact time이다. vel * (1 - ct)
			// 여기에 normal 벡터를 곱해 주어서 방향을 반대로 만든다. contact time <-- next pos
		}

	z.clear();
	pos += vel * fElapsedTime;
}

void Player::DeleteObject()
{
}




