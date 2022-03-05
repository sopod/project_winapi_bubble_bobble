#include "GameManager.h"

GameManager::GameManager()
{
	curScene = Lobby;
	tileSize = 20;
	p = new Player(tileSize);
	m = new Map(tileSize);
	i = new Input();

	p1Score = 0;
}

GameManager::~GameManager()
{
}

RECT GameManager::GetWindowSize()
{
	return { 0, 0, m->GetWindowWidth(tileSize), m->GetWindowHeight(tileSize) };
}

void GameManager::Init() // 0) 게임 초기화
{
}

void GameManager::Update(float fSpeed) // 1) Elapsed Time > 2) 키보드 입력 > 3) 이동(충돌 체크), 삭제여부
{
	

	i->KeyInput();

	//p->MoveObject(fSpeed, m->GetRangeXPos(), *m, tileSize, i->GetHorizontal(), i->GetJump());
	p->MoveObject(fSpeed, m->GetRangeXPos(), m->GetPlatforms(), tileSize, i->GetHorizontal(), i->GetJump());

}

void GameManager::Draw(HDC hdc) // 4) 더블 버퍼링으로 그리기
{
	int width = m->GetWindowWidth(tileSize);
	int height = m->GetWindowHeight(tileSize);

	HDC memDC;
	HBITMAP oldBit, newBit;

	memDC = CreateCompatibleDC(hdc);

	newBit = CreateCompatibleBitmap(hdc, width, height);
	oldBit = (HBITMAP)SelectObject(memDC, newBit);

	Graphics *graphic = new Graphics(memDC);


	// memDC에 게임 그리기 -------------------------------
	switch (curScene)
	{
	case Lobby:

		break;

	case Intro:

		break;

	case Game:

		break;

	case Score:

		break;

	case Gameover:

		break;
	}

	m->DrawMap(graphic, memDC, tileSize);
	p->DrawObject(memDC, tileSize);
	//--------------------------------------------------

	delete graphic;

	BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBit);
	DeleteObject(newBit);

	DeleteDC(memDC);
}
