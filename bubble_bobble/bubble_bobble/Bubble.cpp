#include "Bubble.h"

Bubble::Bubble(float x, float y, bool playerIsRight, B_TYPE type)
{
	img = &Storage::GetInstance().GetBubbleImg(bubbleState, isContainingEnemy, frameMax);
	prevImg = img;
	
	type = type;
	bubbleState = SLIDING;
	isRight = false;

	isSlideDone = false;
	isGoToTopDone = false;


	// 생성 위치
	pos.y = y;
	pos.x = x;
	if (playerIsRight == true && WALLX_RIGHT * TILE_SIZE < x + TILE_SIZE * size)
		pos.x = WALLX_RIGHT * TILE_SIZE - TILE_SIZE * size;
	else if (playerIsRight == false && WALLX_LEFT * TILE_SIZE > x)
		pos.x = WALLX_LEFT * TILE_SIZE;

	// 슬라이드 위치
	goalPos.y = pos.y;
	if (playerIsRight == true)
	{
		SetVel(1, 0);
		goalPos.x = pos.x + SLIDE_MOVEMENT;
	}
	else
	{
		SetVel(-1, 0);
		goalPos.x = pos.x - SLIDE_MOVEMENT;
	}

	isContainingEnemy = false;
	enemyIdx = -1;
}

Bubble::~Bubble() { }

B_STATE Bubble::GetBubbleState() { return bubbleState; }

bool Bubble::GetIsSlideDone() { return isSlideDone; }

int Bubble::GetEnemyIdx() { return enemyIdx; }

bool Bubble::GetIsContainingEnemy() { return isContainingEnemy; }

void Bubble::SetBubbleState(B_STATE state)
{
	bubbleState = state;
	frameCur = frameMin;
}

void Bubble::SetIsContainingEnemy(bool isContainEnemy, int enemyNum)
{
	isContainingEnemy = isContainEnemy;
	enemyIdx =enemyNum;
}

void Bubble::MoveBubble(Scene & s, float speed)
{	
	if (bubbleState == POP) return;

	if (isSlideDone == false)
	{
		MoveToGoal(speed * 2.5f, true);
		if (isMoveToGoalDone == true)
		{
			goalPos = BUBBLE_GOAL;
			if (goalPos.y < pos.y)
				SetVel(0, -1);
			else
				SetVel(0, 1);

			isMoveToGoalDone = false;
			isSlideDone = true;
			bubbleState = NORMAL;
		}
		return;
	}
	
	if (isGoToTopDone == false)
	{
		MoveToGoal(speed * 0.3f, true);

		if (vel.y == 0) SetVelWithGoal(BUBBLE_GOAL);

		if (isMoveToGoalDone == true)
		{
			goalMovement = -TOP_MOVEMENT;
			goalPos.x = pos.x - TOP_MOVEMENT / 2;
			goalPos.y = pos.y;
			SetVelWithGoal(goalPos);
			isMoveToGoalDone = false;
			isGoToTopDone = true;
		}
		return;
	}

	if (isGoToTopDone == true)
	{
		MoveToGoal(speed * 0.3f, true);
		if (isMoveToGoalDone == true)
		{
			goalMovement *= -1;
			goalPos.x += goalMovement;
			SetVelWithGoal(goalPos);
			isMoveToGoalDone = false;
		}
		return;
	}
}

void Bubble::DrawBubble(Graphics * graphic)
{
	img = &Storage::GetInstance().GetBubbleImg(bubbleState, isContainingEnemy, frameMax);

	DrawObject(graphic, 0, 0);
}

void Bubble::UpdateBubbleFrame()
{
	if (bubbleState != SLIDING) frameCounter++;

	newTime = GetTickCount();
	if (bubbleState == NORMAL && newTime - oldTime >= CORROSION1_TIME)
	{
		frameCur = frameMin;
		bubbleState = CORROSION1;
	}
	if (bubbleState == CORROSION1 && newTime - oldTime >= CORROSION2_TIME)
	{
		frameCur = frameMin;
		bubbleState = CORROSION2;
	}
	if (bubbleState == CORROSION2 && newTime - oldTime >= POP_TIME)
	{
		frameCur = frameMin;
		bubbleState = POP;
	}

	// SLIDING이 아니면 프레임 딜레이가 걸린다
	if (bubbleState != SLIDING && BUBBLE_DELAY_MAX != frameCounter)      return;
	else if (bubbleState != SLIDING && BUBBLE_DELAY_MAX == frameCounter) frameCounter = 0;

	frameCur++;

	if (frameCur >= frameMax)
	{
		frameCur = frameMin;
		if (bubbleState == POP)
		{
			life--;
			objState = DEAD;
		}
	}
}
