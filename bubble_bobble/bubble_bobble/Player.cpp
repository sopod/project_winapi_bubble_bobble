#include "Player.h"

Player::Player() 
{ 
	life = START_LIFE;
	InitPlayerAsRoundChangeState(START_ROUND);
}

Player::~Player()
{
	for (auto x : b)
		delete x;
	for (auto x : items)
		delete x;
}

void Player::InitPlayerAsRoundChangeState(int round)
{
	if (round == START_ROUND) InitObjectAsGameState(CENTER_POS);
	else                      InitObjectAsGameState(pos);

	SetVelWithGoal(START_POS * TILE_SIZE);

	doStartMotion = true;
	isDeadMotionDone = false;
	isGameOver = false;
	
	img = &Storage::GetInstance().GetPlayerImg(objState, isAttacking, doStartMotion, frameMax);
	prevImg = img;

	motionRepeatCur = 0;
	b.clear();
	items.clear();
}

std::vector<Bubble*> & Player::GetBubbles() { return b; }

std::vector<Item*> & Player::GetItems() { return items; }

void Player::CreateItem(fPoint enemyPos)
{
	items.push_back(new Item(enemyPos));
}
bool Player::GetIsGameOver() { return isGameOver; }

bool Player::GetIsDeadMotionDone() { return isDeadMotionDone; }

void Player::SetDoStartMotion(bool _DoStartMotion) { doStartMotion = _DoStartMotion; }

void Player::GetHurt()
{
	objState = DEAD;
}

void Player::EatItem(int itemIdx)
{
	items[itemIdx]->SetReadyToShowScore(true);
}

void Player::Attack()
{
	isAttacking = true;
	if (isRight) b.push_back(new Bubble(pos.x + size * TILE_SIZE, pos.y, isRight, DEFAULT));
	else         b.push_back(new Bubble(pos.x - size * TILE_SIZE, pos.y, isRight, DEFAULT));
}

bool Player::PlayerVsBubble(Bubble & b)
{
	// true ���Ͻ� ���� +10

	if (ObjectVsObject(b) == true)
	{
		fPoint nextPos;

		// Bubble�� ���ʿ� �浹
		if (vel.x < 0 && b.GetPos().x + TILE_SIZE * b.GetSize() - pos.x <= BUBBLE_COLLISION_GAP_LEFT)
		{
			nextPos = { pos.x - TILE_SIZE * size + 5, b.GetPos().y };
		}
		// Bubble�� �����ʿ� �浹
		else if (vel.x > 0 && pos.x + TILE_SIZE * size - b.GetPos().x <= BUBBLE_COLLISION_GAP_RIGHT)
		{
			nextPos = { pos.x + TILE_SIZE * size + 5, b.GetPos().y };
		}
		else
		{
			b.SetBubbleState(POP);
			return true;
		}

		// ���� �ε�ġ�� POP, �ƴϸ� �з���
		if (b.ObjectVsSidewall(nextPos) == true)
		{
			b.SetBubbleState(POP);
			return true;
		}
		else
		{
			b.SetPos(nextPos);
			return false;
		}
	}

	return false;
}

void Player::MovePlayer(int moveX, int jumpKey, Scene & s, float fSpeed)
{
	if (objState == DEAD) return;
	
	pos = MoveObject(moveX, jumpKey, s, fSpeed);	

	// ���� �߿��� Bubble�� �浹 üũ �� �Ѵ�
	if (objState == JUMPING) return;

	// Bubble ũ���� ���� ���Ϸ� ������ �浹���� �� �ٴ�ó�� �����Ѵ�
	for (Bubble *& a : b)
	{
		if (ObjectVsObject(*a) == true && vel.y != 0 && pos.y + TILE_SIZE * size - a->GetPos().y <= BUBBLE_COLLISION_JUMP_GAP)
		{
			isOnGround = true;
			return;
		}
	}
}

void Player::DrawPlayer(Graphics * graphic, float gapX, float gapY)
{
	img = &Storage::GetInstance().GetPlayerImg(objState, isAttacking, doStartMotion, frameMax);
	
	DrawObject(graphic, gapX, gapY);
}


void Player::UpdatePlayerFrame()
{
	frameCounter++;
	isDeadMotionDone = false;
	
	// ���� ���� ���� �ð�
	newTime = GetTickCount();
	if (isInvincible == true && newTime - oldTime >= INVINCIBLE_TIME) isInvincible = false;

	// ���� ��, ������ ���� ������ �����̰� ����.
	if (objState != DEAD && isAttacking == false && PLAYER_DELAY_MAX != frameCounter)      return;
	else if (objState != DEAD && isAttacking == false && PLAYER_DELAY_MAX == frameCounter) frameCounter = 0;

	// ������ ����
	frameCur++;

	// �״� ������ �ݺ�
	if (objState == DEAD && frameCur == 12 && motionRepeatCur < MOTION_REPEAT_MAX)
	{
		frameCur = 9;
		motionRepeatCur++;
		return;
	}
	
	// �������� ��� �Ϸ�Ǹ� ó������ �ʱ�ȭ
	if (frameCur >= frameMax)
	{
		frameCur = frameMin;	

		if (isAttacking == true) isAttacking = false;	

		if (objState == DEAD) // �״� �������� ��� �Ϸ�Ǿ��� �� 
		{
			life--;
			isDeadMotionDone = true;
			
			if (life < 0)
			{
				isGameOver = true;
				isDeadMotionDone = false;
				return;
			}

			objState = IDLE;
			isRight = true;
			SetPosWithTileNum(START_POS.x, START_POS.y);

			isInvincible = true;
			oldTime = GetTickCount();
			motionRepeatCur = 0;
			frameCur = frameMin;			
		}
	}
}

