#include "GameCenter.h"

GameCenter::GameCenter()
{
	windowSize = { 0, 0, MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE };
}

GameCenter::GameCenter(const GameCenter & ref) { }

GameCenter::~GameCenter()
{
	delete keyInput;
	delete p;
	delete es;
	Sound::Release();
}


RECT GameCenter::GetWindowSize() { return windowSize; }

bool GameCenter::GetIsExitSelected() { return isExitSelected; }


void GameCenter::Init()
{
	// Screen 
	screen = LOBBY;
	prevScreen = LOBBY;
	isGamePaused = false;
	canEnemyMove = false;
	isEnemyPaused_cheat = false;

	// Pause
	menu_cur = 0;
	isExitSelected = false;
	selectedMenu = -1;

	// Player
	curUser.round = START_ROUND;
	p1Score = 0;
	p2Score = 0;
	isDataSaved = false;

	killOnceCnt = 0;	
	doRoundChange = false;
	enemyMax = 0;

	// Init
	Sound::Init();
	Storage::GetInstance().FileInit();
	p = new Player();
	keyInput = new Input();
	es = new Score();
	Scene::GetInstance().InitScene(START_ROUND);

	// Sound
	isEffectSoundOff = false;
	playJumpSound = true;
	ReplayBgm = false;
	
	bgm = Storage::GetInstance().GetBgm(screen, isHurryUp);
	bgmVol = SOUND_DEFAULT;
	effectVol = SOUND_DEFAULT - EFFECT_VOL_DOWN;
	bgm->play(bgmVol);
	
}

void GameCenter::CreateEnemy()
{
	for (int i = 0; i < enemyMax; i++)
		delete e[i];

	switch (curUser.round)
	{
	case 1:
		enemyMax = 3;
		e[0] = new Enemy(10, 10, false);
		e[1] = new Enemy(15, 10, false);
		e[2] = new Enemy(20, 10, false);
		break;

	case 2:
		enemyMax = 4;
		e[0] = new Enemy(12, 10, false);
		e[1] = new Enemy(18, 10, true);
		e[2] = new Enemy(10, 15, false);
		e[3] = new Enemy(20, 15, true);
		break;

	case 3:
		enemyMax = 4;
		e[0] = new Enemy(10, 5, true);
		e[1] = new Enemy(20, 5, false);
		e[2] = new Enemy(7, 10, true);
		e[3] = new Enemy(23, 10, false);
		break;

	case 4:
		enemyMax = 4;
		e[0] = new Enemy(18, 5, false);
		e[1] = new Enemy(16, 10, false);
		e[2] = new Enemy(14, 15, false);
		e[3] = new Enemy(12, 20, false);
		break;

	case 5:
		enemyMax = 3;
		e[0] = new Enemy(10, 10, false);
		e[1] = new Enemy(15, 10, false);
		e[2] = new Enemy(20, 10, false);
		break;		
	}

	enemyCur = enemyMax;
}

void GameCenter::RoundChange(int round)
{
	if (round == START_ROUND)
		Scene::GetInstance().InitScene(START_ROUND);

	if (round > MAX_ROUND)
	{
		screen = SCORE;
		return;
	}
	screen = ROUNDCHANGE;
	doRoundChange = false;
	curUser.round = round;

	CreateEnemy();
	keyInput->KeyClear();
	InitHurryUp();
	p->InitPlayerAsRoundChangeState(round);
}

void GameCenter::InitHurryUp()
{
	hurryOldTime = GetTickCount();
	isHurryUp = false;
	drawHurryUp = false;
}


void GameCenter::Update(float fSpeed)
{
	// Ű �Է� �ޱ�
	if (p->GetObjectState() != DEAD || p->GetIsGameOver() == true) keyInput->KeyInput();

	// ȭ�� ��ȯ �� -> Ű �Է� ����
	if (prevScreen != screen)
	{
		// RoundChange�� Game�� �뷡�� ����
		if (!(prevScreen == ROUNDCHANGE && screen == GAME))
		{
			bgm->stop();
			bgm = Storage::GetInstance().GetBgm(screen, isHurryUp);
			bgm->play(bgmVol);
		}
		keyInput->KeyClear();
		prevScreen = screen;
	}

	// ESC ��ȯ���� Pause �޴� ����
	if (keyInput->GetEsc() == 1)
	{
		menu_cur = 0;
		selectedMenu = -1;
		isGamePaused = (isGamePaused == true) ? false : true;
		hurryOldTime = GetTickCount();
	}

	// Pause �޴�
	if (isGamePaused == true)
	{
		hurryOldTime = GetTickCount();

		if (keyInput->GetUpDown() == -1)
		{
			menu_cur--;
			if (menu_cur < 0) menu_cur = MENU_MAX;
		}
		else if (keyInput->GetUpDown() == 1)
		{
			menu_cur++;
			if (menu_cur > MENU_MAX) menu_cur = 0;
		}

		if (keyInput->GetEnter() == 1)
		{
			selectedMenu = menu_cur;

			switch(menu_cur)
			{
			case 0:
				isGamePaused = false;
				menu_cur = 0;
				break;

			case 1:
				{
					// Init
					isGamePaused = false;
					canEnemyMove = false;
					isEnemyPaused_cheat = false;

					menu_cur = 0;
					isExitSelected = false;
					selectedMenu = -1;

					curUser.round = START_ROUND;
					name = "";
					p1Score = 0;
					p2Score = 0;
					isDataSaved = false;

					killOnceCnt = 0;
					doRoundChange = false;
					enemyMax = 0;

					isEffectSoundOff = false;
					playJumpSound = true;
					ReplayBgm = false;

					p->SetLife(START_LIFE);
					p->InitPlayerAsRoundChangeState(START_ROUND);

					isGamePaused = false;
					menu_cur = 0;
					screen = LOBBY;
				}				
				break;

			case 2:
				isGamePaused = false;
				menu_cur = 0;
				isExitSelected = true;
				break;

			case 3:
				bgm->volumeUp();
				bgmVol = bgm->GetVolume();
				effectVol = (bgmVol + EFFECT_VOL_DOWN >= 0.9f) ? 0.9f : bgmVol + EFFECT_VOL_DOWN;
				break;

			case 4:
				bgm->volumeDown();
				bgmVol = bgm->GetVolume();
				effectVol = (bgmVol - EFFECT_VOL_DOWN <= 0.1f) ? 0.1f : bgmVol - EFFECT_VOL_DOWN;
				break;

			case 5:
				isEffectSoundOff = (isEffectSoundOff == true) ? false : true;
			}
		}
		return;
	}	


	switch (screen)
	{
	case LOBBY:
	{		
		if (keyInput->GetEnter() == 1)
		{
			RoundChange(START_ROUND);
		}
	}
		break;

	case GAME:
	{
		// P������ Enemy ���߱�
		if (keyInput->GetChar() == 'P')
			isEnemyPaused_cheat = (isEnemyPaused_cheat == true) ? false : true;

		// hurry_time�� �Ѿ�� Hurry Up �����ϱ�
		if (isHurryUp == false && doRoundChange == false)
		{
			hurryNewTime = GetTickCount();
			if (hurryNewTime - hurryOldTime >= HURRY_TIME)
			{
				isHurryUp = true;
				drawHurryUp = true;
				// Enemy ȭ��
				for (int i = 0; i < enemyMax; i++)
					if (e[i]->GetNotExists() == false)
						e[i]->SetAngry(true);

				// Hurry Up ����
				bgmOldTime = GetTickCount();
				bgm->stop();
				Storage::GetInstance().GetBgmEffect()->play(effectVol);
				ReplayBgm = true;
			}
		}
		else if (isHurryUp == true && ReplayBgm == true)
		{
			// Hurry Up ���尡 ������ ������� �ٽ� ����ϱ�
			bgmNewTime = GetTickCount();
			if (bgmNewTime - bgmOldTime >= BGM_TIME)
			{
				bgm = Storage::GetInstance().GetBgm(screen, isHurryUp);
				bgm->play(bgmVol);
				ReplayBgm = false;
			}
		}

		// Enemy�� ��� ������ ROUND_CHANGE_TIME ��ŭ ��ٷȴٰ� ���� ����� �Ѿ��
		if (doRoundChange == true)
		{
			roundChangeNewTime = GetTickCount();
			if (roundChangeNewTime - roundChangeOldTime >= ROUND_CHANGE_TIME)
			{
				RoundChange(curUser.round + 1);
				return;
			}
		}		

		// Player �����̱�
		p->MovePlayer(keyInput->GetHorizontal(), keyInput->GetJump(), Scene::GetInstance(), fSpeed);

		// ���� ���� ����ϱ�
		if (isEffectSoundOff == false)
		{
			if (playJumpSound == false && p->GetIsOnGround() == true)
			{
				playJumpSound = true;
			}
			if (playJumpSound == true && keyInput->GetJump() == 1 && p->GetObjectState() == JUMPING)
			{
				Storage::GetInstance().GetPlayerSound(JUMP)->play(effectVol);
				playJumpSound = false;
			}			
		}

		// Enemy �����̱�
		for (int i = 0; i < enemyMax; i++)
		{
			if (e[i]->GetNotExists() == false)
			{
				// Enemy�� ���ߴ� ����: cheatŰ �Է� or Ready �� 
				if ((isEnemyPaused_cheat == true || canEnemyMove == false) && (e[i]->GetObjectState() != DEAD && e[i]->GetIsOnGround() == true))
					continue;
				e[i]->MoveEnemy(p->GetPos(), Scene::GetInstance(), fSpeed);
			}
		}

		// Bubble �����̱�
		for (Bubble* & z : p->GetBubbles())
			z->MoveBubble(Scene::GetInstance(), fSpeed);

		// Item �����̱�
		for (int i = 0; i < p->GetItems().size(); i++)
		{
			if (p->GetItems()[i]->GetIsMoveToGoalDone() == false)
			{
				p->GetItems()[i]->MoveItem(fSpeed);
			}
			else
			{
				p->GetItems().erase(p->GetItems().begin() + i);
				i--;
			}
		}

		// Enemy Score �����̱�
		if (es->GetIsMoveToGoalDone() == false) es->MoveScore(fSpeed);
		
		// Bubble ����
		if (keyInput->GetAttack() == 1)
		{
			if(isEffectSoundOff == false) Storage::GetInstance().GetPlayerSound(ATTACK)->play(effectVol);
			p->Attack();
		}
		
		// Enemy vs Player -> Player ��ħ
		if (p->GetObjectState() != DEAD && p->GetIsInvincible() == false)
		{
			for (int i = 0; i < enemyMax; i++)
			{
				if (e[i]->GetObjectState() != DEAD && e[i]->GetNotExists() == false && e[i]->ObjectVsObject(*p) == true)
				{
					if (isEffectSoundOff == false) Storage::GetInstance().GetPlayerSound(GOODBYE)->play(effectVol);

					p->GetHurt();
					//keyInput->KeyClear();
					break;
				}
			}
		}		

		// Bubbe vs Enemy -> Enemy ����
		for (Bubble* & z : p->GetBubbles())
		{
			if (z->GetIsContainingEnemy() == true || z->GetIsSlideDone() == true) continue;

			for (int i = 0; i < enemyMax; i++)
			{
				if (e[i]->GetObjectState() != DEAD && z->ObjectVsObject(*(e[i])) == true && e[i]->GetIsCaptured() == false)
				{
					e[i]->SetIsCaptured(true);
					z->SetIsContainingEnemy(true, i);
					break;
				}
			}
		}

		// Player vs Bubble -> Bubble ����
		int kill_mon = 0;
		int pop_bubble = 0;
		for (int i = 0; i < p->GetBubbles().size(); i++)
		{
			if (p->GetBubbles()[i]->GetBubbleState() == POP) continue;

			if (p->PlayerVsBubble(*(p->GetBubbles()[i])) == true)
			{
				if (p->GetBubbles()[i]->GetBubbleState() != POP) continue;

				pop_bubble++;

				// Enemy�� ����
				if (p->GetBubbles()[i]->GetIsContainingEnemy() == true)
				{
					if (isEffectSoundOff == false) Storage::GetInstance().GetPlayerSound(POPBUBBLE)->play(effectVol);

					e[p->GetBubbles()[i]->GetEnemyIdx()]->SetDie(p->GetBubbles()[i]->GetPos(), p->GetIsRight());
					p->GetBubbles()[i]->SetIsContainingEnemy(false, -1);

					kill_mon++;
					pop_bubble--;
				}
			}
		}

		// Score �ø���
		p1Score += (pop_bubble * 10);

		killOnceCnt++;
		killOnce += kill_mon;

		if (killOnceCnt >= KILL_ONCE_TIMING)
		{
			p1Score += (1000 * killOnce);
			if (killOnce >= 2) es->InitScore(p->GetPos(), 1000 * killOnce, SCORE_MOVEMENT);
			killOnceCnt = 0;
			killOnce = 0;
		}


		// Bubble�� �ð��� ������(Life�� 0�̸�) ����
		for (int i = 0; i < p->GetBubbles().size(); i++)
		{
			if (p->GetBubbles()[i]->GetLife() == 0)
			{
				if (p->GetBubbles()[i]->GetIsContainingEnemy() == true)
					e[p->GetBubbles()[i]->GetEnemyIdx()]->SetReleased(p->GetBubbles()[i]->GetPos());

				p->GetBubbles().erase(p->GetBubbles().begin() + i);
				i--;
			}
		}

		// Bubble�� POP ���� �� ����
		for (int i = 0; i < p->GetBubbles().size(); i++)
		{
			if (p->GetBubbles()[i]->GetObjectState() == DEAD)
			{
				p->GetBubbles().erase(p->GetBubbles().begin() + i);
				i--;
			}
		}

		// Enemy�� DEAD ������ �� �ٴڰ� �浹�ϸ� Item�� �ǰ�, ���ŵȴ�
		for (int i = 0; i < enemyMax; i++)
		{
			if (e[i]->GetReadyToBeItem() == true && e[i]->GetNotExists() == false)
			{
				p->CreateItem(e[i]->GetPos()); // Item�� ����

				e[i]->SetNotExists(true);
				enemyCur--;
			}
		}

		// Player vs Item -> Item �Ա�
		for (int i = 0; i < p->GetItems().size(); i++)
		{
			if (p->GetItems()[i]->GetReadyToShowScore() == false && p->ObjectVsObject(*(p->GetItems()[i])) == true)
			{
				if (isEffectSoundOff == false)
					Storage::GetInstance().GetPlayerSound(GETITEM)->play(effectVol);
				p1Score += p->GetItems()[i]->GetItemScore();
				p->EatItem(i);
			}
		}

		// Enemy ��� ���� �� Round Change
		if (enemyCur == 0 && doRoundChange == false)
		{
			doRoundChange = true;
			roundChangeOldTime = GetTickCount();
		}

		// ���� ���� Ȯ��
		if (p->GetIsGameOver() == true) screen = SCORE;
	}
		break;

	case ROUNDCHANGE:
	{
		p->MoveToGoal(fSpeed * 1.3f, false);

		for (int i = 0; i < enemyMax; i++)
			if (e[i]->GetNotExists() == false) e[i]->MoveToGoal(fSpeed, false);
	}
		break;

	case SCORE:
	{
		if (isDataSaved == true) return;

		// �齺���̽��� �����
		if (keyInput->GetBackspace() == 1 && name.size() != 0)  name.pop_back();

		// ���ڴ� �̸� �Է�
		char chr = keyInput->GetChar();

		if (name.size() < 3 && chr != '0') name += chr;

		// ���ʹ� ���̺�
		if (name.size() >= 1 && keyInput->GetEnter() == 1)
		{
			isDataSaved = true;
			curUser.score = p1Score;
			int i = 0;
			while (name[i] != '\0')
			{
				curUser.name[i] = name[i];
				i++;
			}
			curUser.name[i] = '\0';

			Storage::GetInstance().SaveTopFiveDataFile(curUser);
			Storage::GetInstance().LoadDataFile();

			if (p->GetIsGameOver() == true) screen = GAMEOVER;
		}

		keyInput->KeyClear();
	}
		break;

	case GAMEOVER: 
	{

	}
		break;
	}

	bgm->Update();
}

void GameCenter::UpdateFrame()
{
	if (isGamePaused == true) return;

	p->UpdatePlayerFrame();
	
	// Player�� DEAD ����� ��� ������ 
	if (p->GetIsDeadMotionDone() == true)
	{
		if (isHurryUp == true)
		{
			bgm->stop();
			bgm = Storage::GetInstance().GetBgm(screen, false);
			bgm->play(bgmVol);
		}
		InitHurryUp();
		for (int i = 0; i < enemyMax; i++)
			e[i]->SetAngry(false);
	}

	for (int i = 0; i < enemyMax; i++)
		if (e[i]->GetNotExists() == false) 
			e[i]->UpdateEnemyFrame();

	for (Bubble* & z : p->GetBubbles())
		z->UpdateBubbleFrame();

	if (drawHurryUp == true)
		Scene::GetInstance().UpdateSceneFrame(drawHurryUp);
}


void GameCenter::Draw(HDC hdc)
{
	HDC memDC;
	HBITMAP oldBit, newBit;

	memDC = CreateCompatibleDC(hdc);

	newBit = CreateCompatibleBitmap(hdc, windowSize.right, windowSize.bottom);
	oldBit = (HBITMAP)SelectObject(memDC, newBit);

	Graphics graphic(memDC);

	//-------------------------------------------------------------------------------
	switch (screen)
	{
	case LOBBY:
	{
		Scene::GetInstance().DrawLobbyScreen(&graphic, 0);
	}
		break;

	case GAME:
	{
		Scene::GetInstance().DrawGameScreen(&graphic, curUser.round, p->GetLife(), p1Score, p2Score);

		for (int i = 0; i < enemyMax; i++)
			if (e[i]->GetNotExists() == false) 
				e[i]->DrawEnemy(&graphic);

		p->DrawPlayer(&graphic, 0, 0);

		for (Bubble* & z : p->GetBubbles())
			z->DrawBubble(&graphic);

		for (Item * & x : p->GetItems())
			x->DrawItem(&graphic);

		if (es->GetIsMoveToGoalDone() == false) es->DrawScore(&graphic, 3);

		if (drawHurryUp == true) Scene::GetInstance().DrawHurryUp(&graphic);

		if (Scene::GetInstance().DrawReady(&graphic, curUser.round) == true) canEnemyMove = true;
	}
		break;

	case ROUNDCHANGE:
	{	
		// �� �׸���
		if (Scene::GetInstance().DrawRoundChange(&graphic, curUser.round, p->GetLife(), p1Score, p2Score, isGamePaused) == true)
		{
			screen = GAME;
			canEnemyMove = false;
			p->SetDoStartMotion(false);
			//keyInput->KeyClear();
			return;
		}

		// ������Ʈ �׸���
		p->DrawPlayer(&graphic, TILE_SIZE, TILE_SIZE);

		for (int i = 0; i < enemyMax; i++)
			if (e[i]->GetNotExists() == false) 
				e[i]->DrawEnemy(&graphic);

	}
		break;

	case SCORE:
	{
		Scene::GetInstance().DrawScoreScreen(&graphic, curUser.round, p1Score, name);
	}
		break;

	case GAMEOVER:
	{
		Scene::GetInstance().DrawGameOverScreen(&graphic);
	}
		break;
	}

	if (isGamePaused == true)
	{
		Scene::GetInstance().DrawPauseScreen(&graphic, menu_cur, selectedMenu, bgm->GetVolume() * 10, isEffectSoundOff);
		selectedMenu = -1;
	}
	//-------------------------------------------------------------------------------

	BitBlt(hdc, 0, 0, windowSize.right, windowSize.bottom, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBit);
	DeleteObject(newBit);

	DeleteDC(memDC);
}
