#pragma once
#include "Object.h"
#include "Bubble.h"
#include "Item.h"

class Player : public Object
{
private:
	std::vector<Bubble *> b;
	std::vector<Item *> items;
	
	bool doStartMotion;
	bool isDeadMotionDone;
	bool isGameOver;

	int motionRepeatCur;

public:
	Player();
	~Player();	   

	void InitPlayerAsRoundChangeState(int round);

	std::vector<Bubble *> & GetBubbles();
	std::vector<Item *> & GetItems();
	void CreateItem(fPoint enemyPos);

	bool GetIsGameOver();
	bool GetIsDeadMotionDone();
	void SetDoStartMotion(bool doStartMotion);

	void GetHurt();
	void EatItem(int itemIdx);
	void Attack();
	
	bool PlayerVsBubble(Bubble & b);

	void MovePlayer(int moveX, int jumpKey, Scene & s, float speed);
	
	void DrawPlayer(Graphics * graphic, float gapX, float gapY);
	void UpdatePlayerFrame();

};