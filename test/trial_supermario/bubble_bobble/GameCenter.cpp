#include "GameCenter.h"
#include "Input.h"

GameCenter::GameCenter()
{
	windowSize = { 0, 0, 500, 500 };
	int tileSize = 20;
	s = new Screen(tileSize);
	p = new Player();
	i = new Input();
}
	
GameCenter::~GameCenter()
{
}

RECT GameCenter::GetWindowSize()
{
	return windowSize;
}

void GameCenter::Update(float fElapsedTime)
{
	i->KeyInput();
	p->MovePlayer(i->GetHorizontal(), i->GetJump(), s->GetTileSize(), *s, fElapsedTime);
	s->SetCamera(p->GetPos());
}

void GameCenter::Draw(HDC hdc)
{
	HDC memDC;
	HBITMAP oldBit, newBit;

	memDC = CreateCompatibleDC(hdc);

	newBit = CreateCompatibleBitmap(hdc, windowSize.right, windowSize.bottom);
	oldBit = (HBITMAP)SelectObject(memDC, newBit);

	Graphics *graphic = new Graphics(memDC);

	//-------------------------------------------------------------------------------
	s->DrawMap(graphic);
	p->DrawPlayer(graphic, s->GetTileSize(), s->GetOffset());
	//-------------------------------------------------------------------------------

	delete graphic;

	BitBlt(hdc, 0, 0, windowSize.right, windowSize.bottom, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBit);
	DeleteObject(newBit);

	DeleteDC(memDC);
}
