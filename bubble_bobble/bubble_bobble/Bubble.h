#pragma once
#include "Object.h"

class Bubble : public Object
{
private:
	B_TYPE type;
	B_STATE bubbleState;

	int goalMovement;

	bool isSlideDone;
	bool isGoToTopDone;

	int enemyIdx;
	bool isContainingEnemy;

public:
	Bubble(float x, float y, bool playerIsRight, B_TYPE type);
	~Bubble();

	B_STATE GetBubbleState();
	bool GetIsSlideDone();
	int GetEnemyIdx();
	bool GetIsContainingEnemy();
	
	void SetBubbleState(B_STATE bubbleState);
	void SetIsContainingEnemy(bool isContainEnemy, int enemyNum);
	
	void MoveBubble(Scene &s, float speed);
	
	void DrawBubble(Graphics *graphic);
	void UpdateBubbleFrame();
};

