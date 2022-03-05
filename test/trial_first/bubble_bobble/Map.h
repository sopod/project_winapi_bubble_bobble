#pragma once
#include "framework.h"
#include <string>

class Map
{
protected:
	std::wstring map;
	std::pair<int, int> rangeXPos;
	std::vector<FPOINT> platforms;

	int mapWidth;   // Ÿ�� ����
	int mapHeight;

public:
	Map(int tileSize);
	~Map();

	int GetWindowWidth(int tileSize);
	int GetWindowHeight(int tileSize);

	std::pair<int, int> GetRangeXPos();
	std::vector<FPOINT> GetPlatforms();

	void SetLevel1(int tileSize);

	wchar_t GetTileFromLoc(int x, int y); // Ÿ�� ��ȣ -> Ÿ�� ����
	void SetTileFromLoc(int x, int y, wchar_t c); // Ÿ�� ��ȣ, Ÿ�� ���� -> ����

	wchar_t GetTileFromPos(float x, float y, int tileSize);
	POINT Collision(float x, float y, int tileSize);

	void DrawMap(Graphics * graphic, HDC hdc, int tileSize);
};

