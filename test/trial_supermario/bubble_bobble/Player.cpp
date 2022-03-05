#include "Player.h"

Player::Player()
{
	pos = { 0, 0 };
	moveSpeed = 0.1f; // � Ÿ��
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
	
		// �������� �̵� ��
		if (vel.x <= 0)
		{
			// �»�ܰ� ���ϴ��� #���
			// �ٷ� ������ �� Ÿ�ϸ� ���� ���� 1.0f 1.9f�� ���Ѵ�. 2.0f�̶�� ���� Ÿ���� �� ���� �ֱ� �����̴�. (2 = �÷��̾��� Ÿ���� 2ĭ)
			if (s.GetTileFromIdx(nextPos.x, pos.y) != L'.' || s.GetTileFromIdx(nextPos.x, pos.y + 1.0f) != L'.' || s.GetTileFromIdx(nextPos.x, pos.y + 1.9f) != L'.')
			{
				// ��� ���� Ÿ���� int������, ������ �����Ƿ� int������ cast�Ѵ�.
				// �ٷ� ���� ��ġ�� Ÿ�� 1ĭ �ڷΰ���.
				nextPos.x = (int)nextPos.x + 1;
				vel.x = 0;
			}
		}	
		// ���������� �̵� ��
		else
		{
			// ���ܰ� ���ϴ��� #���
			if (s.GetTileFromIdx(nextPos.x + 2.0f, pos.y) != L'.' || s.GetTileFromIdx(nextPos.x + 2.0f, pos.y + 1.0f) != L'.' || s.GetTileFromIdx(nextPos.x + 2.0f, pos.y + 1.9f) != L'.')
			{
				nextPos.x = (int)nextPos.x;
				vel.x = 0;
			}
		}

		IsOnGround = false;

		// ���� �̵� ��
		if (vel.y <= 0)
		{
			if (s.GetTileFromIdx(nextPos.x, nextPos.y) != L'.' || s.GetTileFromIdx(nextPos.x + 1.0f, nextPos.y) != L'.' || s.GetTileFromIdx(nextPos.x + 1.9f, nextPos.y) != L'.')
			{
				nextPos.y = (int)nextPos.y + 1;
				vel.y = 0;
			}
		}
		// �Ʒ��� �̵� ��
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
	graphic->FillRectangle(&brush, (int)(pos.x - offset.x )* tileSize, (int)(pos.y - offset.y )* tileSize, tileSize * size, tileSize * size); // �»��
}
