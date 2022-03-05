#include "Player.h"

Player::Player()
{
	pos = { 0, 0 };
	moveSpeed = 0.1f; // 몇개 타일
	jumpPower = -4.0f;

	size = 2;
	life = 3;
}

Player::~Player()
{
}

void Player::MovePlayer(int moveX, int jump, int tileSize, Screen & s, float fElapsedTime)
{
	vel.x = moveX * moveSpeed * fElapsedTime;

	if (jump != 0 && IsOnGround == true)
		vel.y = jump * jumpPower * fElapsedTime;

	vel.y += gravity * fElapsedTime;

	FPOINT nextPos = pos + vel;
	
		// 왼쪽으로 이동 시
		if (vel.x <= 0)
		{
			// 좌상단과 좌하단이 #라면
			// 바로 왼쪽의 맵 타일만 보기 위해 1.0f 1.9f를 더한다. 2.0f이라면 다음 타일을 볼 수도 있기 때문이다. (2 = 플레이어의 타일은 2칸)
			if (s.GetTileFromIdx(nextPos.x, pos.y) != L'.' || s.GetTileFromIdx(nextPos.x, pos.y + 1.0f) != L'.' || s.GetTileFromIdx(nextPos.x, pos.y + 1.9f) != L'.')
			{
				// 모든 맵의 타일은 int형으로, 나눠져 있으므로 int형으로 cast한다.
				// 바로 직전 위치인 타일 1칸 뒤로가기.
				nextPos.x = (int)nextPos.x + 1;
				vel.x = 0;
			}
		}	
		// 오른쪽으로 이동 시
		else
		{
			// 우상단과 우하단이 #라면
			if (s.GetTileFromIdx(nextPos.x + 2.0f, pos.y) != L'.' || s.GetTileFromIdx(nextPos.x + 2.0f, pos.y + 1.0f) != L'.' || s.GetTileFromIdx(nextPos.x + 2.0f, pos.y + 1.9f) != L'.')
			{
				nextPos.x = (int)nextPos.x;
				vel.x = 0;
			}
		}

		IsOnGround = false;

		// 위로 이동 시
		if (vel.y <= 0)
		{
			if (s.GetTileFromIdx(nextPos.x, nextPos.y) != L'.' || s.GetTileFromIdx(nextPos.x + 1.0f, nextPos.y) != L'.' || s.GetTileFromIdx(nextPos.x + 1.9f, nextPos.y) != L'.')
			{
				nextPos.y = (int)nextPos.y + 1;
				vel.y = 0;
			}
		}
		// 아래로 이동 시
		else
		{
			if (s.GetTileFromIdx(nextPos.x, nextPos.y + 2.0f) != L'.' || s.GetTileFromIdx(nextPos.x + 1.0f, nextPos.y + 2.0f) != L'.' || s.GetTileFromIdx(nextPos.x + 1.9f, nextPos.y + 2.0f) != L'.')
			{
				nextPos.y = (int)nextPos.y;
				vel.y = 0;
				IsOnGround = true;
			}
		}

	pos = nextPos;

}

void Player::DrawPlayer(Graphics * graphic, int tileSize, FPOINT offset)
{
	SolidBrush brush(Color(255, 255, 255, 255));
	graphic->FillRectangle(&brush, (int)(pos.x - offset.x )* tileSize, (int)(pos.y - offset.y )* tileSize, tileSize * size, tileSize * size); // 좌상단
}
