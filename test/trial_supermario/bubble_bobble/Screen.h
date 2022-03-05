#pragma once
#include "framework.h"

class Screen
{
protected:
	std::wstring map;
	std::pair<int, int> RangeXPos;

	POINT mapTiles;
	POINT visibleTiles;
	int tileSize;

	FPOINT offset;
	FPOINT tileOffset;

	FPOINT camera; // 화면의 정중앙

public:
	Screen(int _tileSize);
	~Screen();

	std::pair<int, int> GetRangeXPos();
	int GetTileSize();
	FPOINT GetOffset();

	void SetCamera(FPOINT _pos);

	void SetLevel1();

	wchar_t GetTileFromIdx(int x, int y);
	void SetTileFromIdx(int x, int y, wchar_t c);

	void DrawMap(Graphics * graphic);
};

