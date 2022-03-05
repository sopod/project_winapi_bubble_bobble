#include "Map.h"
#include "Draw.h"

Map::Map(int tileSize)
{
	mapWidth = 32;
	mapHeight = 30;
	rangeXPos = { 2 * tileSize, (mapWidth - 2) * tileSize };
	SetLevel1(tileSize);
}

Map::~Map()
{
}

int Map::GetWindowWidth(int tileSize)
{
	return mapWidth * tileSize;
}

int Map::GetWindowHeight(int tileSize)
{
	return mapHeight * tileSize;
}

std::pair<int, int> Map::GetRangeXPos()
{
	return rangeXPos;
}

std::vector<FPOINT> Map::GetPlatforms()
{
	return platforms;
}


void Map::SetLevel1(int tileSize)
{
	/*
	 map = L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";

	map += L"...#######......................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";
	*/
	
	map += L"................................";
	map += L"................................";
	map += L"................................";
	map += L"................................";

	map += L"################################";
	map += L"##............................##"; 
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"####....################....####";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"####....################....####";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"####....################....####";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"##............................##";
	map += L"################################";
	
	platforms.clear(); // x = 2 ~ width - 3, y = 5 ~ height - 1
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			if (GetTileFromLoc(x, y) == L'#' && x >= 2 && x <= mapWidth - 3 && y >= 4 && y <= mapHeight - 1)
			{
				platforms.push_back({ (float)x * tileSize, (float)y * tileSize });
			}
		}
	}
}

wchar_t Map::GetTileFromLoc(int x, int y)
{
	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
		return map[y * mapWidth + x];
	else
		return L' ';
}

void Map::SetTileFromLoc(int x, int y, wchar_t c)
{
	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
		map[y * mapWidth + x] = c;
}

wchar_t Map::GetTileFromPos(float x, float y, int tileSize)
{
	int tileY = y / tileSize;
	int tileX = x / tileSize;
	return map[tileY * mapWidth + tileX];
}

POINT Map::Collision(float x, float y, int tileSize)
{
	POINT tile;
	tile.y = y / tileSize;
	tile.x = x / tileSize;
	/*
	if (tile.y >= mapHeight || tile.x >= mapWidth)
	{
		while (tile.y >= mapHeight)
		{
			tile.y -= 1;
		}
		while (tile.x >= mapWidth)
		{
			tile.x -= 1;
		}
		return tile;
	}*/
	if (map[tile.y * mapWidth + tile.x] == L'#')
		return tile;
	else
		return {-1,-1 };
}





void Map::DrawMap(Graphics * graphic, HDC hdc, int tileSize)
{
	SolidBrush brush1(Color(255, 0, 0, 0));
	SolidBrush brush2(Color(255, 255, 0, 0));

	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			wchar_t curTile = GetTileFromLoc(x, y);
			switch (curTile)
			{
			case L'.':
				//graphic->FillRectangle(&brush1, x * tileSize, y * tileSize, tileSize, tileSize);
				DrawRectColor(hdc, { x * tileSize, y * tileSize }, tileSize, 0, 0, 0);
				break;

			case L'#':
				//graphic->FillRectangle(&brush1, x * tileSize, y * tileSize, tileSize, tileSize);
				DrawRectColor(hdc, { x * tileSize, y * tileSize }, tileSize, 255, 0, 0);
				break;
			}
		}
	}
}
