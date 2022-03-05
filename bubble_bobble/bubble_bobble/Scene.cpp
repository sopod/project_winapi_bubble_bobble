#include "Scene.h"

Scene::Scene()
{
	charImg = &Storage::GetInstance().GetCharImg();
	InitScene(START_ROUND);
}

Scene::Scene(const Scene & ref) { }

Scene::~Scene() { }

void Scene::InitScene(int round)
{	
	// 라운드에 따른 초기 상태
	prevRound = round;

	roundChangeCnt = 0;
	frameCur = FRAME_MIN;
	readyCnt = 0;
	delayCnt = 0;
	repeatCnt = 0;

	SetRound(round);
}

void Scene::SetRound(int round)
{
	prevTileImg = curTileImg;
	curTileImg = Storage::GetInstance().GetTileImg()[TILE_NUMBER[round-1]];
	prevMap = curMap;

	switch (round)
	{
	case 1:
		curTileImg = Storage::GetInstance().GetTileImg()[TILE_NUMBER[round-1]];
		prevTileImg = curTileImg;
				
		curMap = L"................................";
		curMap += L"................................";

		curMap += L"################################";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"####....################....####";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"####....################....####";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"####....################....####";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"################################";
		prevMap = curMap;
		return;

	case 2:		
		curMap = L"................................";
		curMap += L"................................";

		curMap += L"################################";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##..........########..........##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##......######....######......##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##....####################....##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##..#######...#####...######..##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"################################";
		return;	

	case 3:
		curMap = L"................................";
		curMap += L"................................";

		curMap += L"#########....######....#########";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##....##########################";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##########################....##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##....##########################";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##..##########....##########..##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"##............................##";
		curMap += L"#########....######....#########";
		return;
	}

}

POINT Scene::GetTileWithPos(fPoint pos)
{
	int tileX = pos.x / TILE_SIZE;
	int tileY = pos.y / TILE_SIZE;
	return { tileX, tileY };
}

wchar_t Scene::GetTileWithIdx(int x, int y)
{
	if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) return curMap[y * MAP_WIDTH + x];
	else return L' ';
}

wchar_t Scene::GetTileWithIdxAndMap(int x, int y, std::wstring * _map)
{
	if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) return (*_map)[y * MAP_WIDTH + x];
	else return L' ';
}

void Scene::SetTileWithIdx(int x, int y, wchar_t c)
{
	if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
		curMap[y * MAP_WIDTH + x] = c;
}

bool Scene::PosVsTile(float x, float y) // fPoint 위치에 벽타일#이 있는가 -> bool
{
	int tileX = x / TILE_SIZE;
	int tileY = y / TILE_SIZE;

	if (tileX < 0 || tileX >= MAP_WIDTH)      return false;
	if (tileY < 0 || tileY >= MAP_HEIGHT)     return false;
	if (GetTileWithIdx(tileX, tileY) == L'#') return true;
	return false;
}

fPoint Scene::PosVsTileWhere(float x, float y) 
{
	// fPoint -> fPoint
	// 맞닿은 벽의 좌상단 위치 반환

	int tileX = x / TILE_SIZE;
	int tileY = y / TILE_SIZE;
	
	// x축 양 끝 벽은 통과 못한다
	if (tileX < WALLX_LEFT || (WALLX_RIGHT - 1) < tileX)
		return { (float)(tileX * TILE_SIZE), (float)(tileY * TILE_SIZE) };
	// y축 맨위는 통과한다
	else if (tileY <= WALLY)
		return { 0.0f, 0.0f };
	// 나머지 경우는 벽일 때만 통과 못한다
	else if (GetTileWithIdx(tileX, tileY) == L'#')
		return { (float)(tileX * TILE_SIZE), (float)(tileY * TILE_SIZE) };
	
	return { 0.0f, 0.0f };
}

int Scene::GetCharPos(char c)
{
	int charNum = (c - '!');

	return CHAR_GAP * charNum + charNum + 1;
}

void Scene::DrawChar(Graphics * graphic, int x, int y, COLOR color, char c)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(0, 0, 0), Color(0, 0, 0));

	int width = CHAR_GAP;
	int height = (*charImg)[color]->GetHeight();

	Rect dest = { x * TILE_SIZE, y * TILE_SIZE, width * 2, height * 2 };

	graphic->DrawImage((*charImg)[color], dest, GetCharPos(c), 0, width, height, UnitPixel, &imgAttr);
}

void Scene::DrawString(Graphics * graphic, int x, int y, COLOR color, const std::string & str)
{
	for (int i = 0; i < str.size(); i++)
		DrawChar(graphic, x + i, y, color, str[i]);
}

void Scene::DrawNumber(Graphics * graphic, int rightX, int y, COLOR color, int & n)
{
	// (오른쪽정렬)
	std::string str = std::to_string(n);
	for (int i = 0; i < str.size(); i++)
		DrawChar(graphic, rightX - (str.size() - i - 1), y, color, str[i]);
}

void Scene::DrawTile(Graphics * graphic, bool IsPrevRound, int x, int y)
{
	if (IsPrevRound == true)
	{
		int width = prevTileImg->GetWidth();
		int height = prevTileImg->GetHeight();
		Rect dest = { x, y, width * 2, height * 2 };

		graphic->DrawImage(prevTileImg, dest,
			0, 0, width, height,
			UnitPixel);
	}
	else
	{
		int width = curTileImg->GetWidth();
		int height = curTileImg->GetHeight();
		Rect dest = { x, y, width * 2, height * 2 };

		graphic->DrawImage(curTileImg, dest,
			0, 0, width, height,
			UnitPixel);
	}

}

void Scene::DrawMap(Graphics * graphic)
{
	SolidBrush brush(Color(255, 0, 0, 0));

	for (int x = 0; x < MAP_WIDTH; x++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			wchar_t curTile = GetTileWithIdx(x, y);
			switch (curTile)
			{
			case L'.': // 배경
				graphic->FillRectangle(&brush, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE); // 좌상단
				break;

			case L'#': // 플랫폼
				DrawTile(graphic, false, x * TILE_SIZE, y * TILE_SIZE);
				break;
			}
		}
	}
}

// startY만큼 아래에 출력된다.
void Scene::DrawMap(Graphics * graphic, bool IsPrevRound, int startY)
{
	SolidBrush brush(Color(255, 0, 0, 0));
	std::wstring * map;

	if (IsPrevRound == true)
	{
		map = &prevMap;
	}
	else
	{
		map = &curMap;

		// 스코어 표시되는 맨위 두줄
		for (int x = 0; x < MAP_WIDTH; x++)
			for (int y = 0; y < 2; y++)
				graphic->FillRectangle(&brush, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	}	

	for (int x = 0; x < MAP_WIDTH; x++)
	{
		for (int y = 2; y < MAP_HEIGHT; y++)
		{
			wchar_t curTile = GetTileWithIdxAndMap(x, y, map);
			switch (curTile)
			{
			case L'.':
				graphic->FillRectangle(&brush, x * TILE_SIZE, startY + y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
				break;

			case L'#': 
				DrawTile(graphic, IsPrevRound, x* TILE_SIZE, startY + y * TILE_SIZE);
				break;
			}
		}
	}
}


void Scene::DrawPlayerLife(Graphics *graphic, int x, int y, int idx)
{
	Image * img = Image::FromFile(L"sprite/p1/p1life.png");
	int width = img->GetWidth();
	int height = img->GetHeight();

	Rect dest = { x * TILE_SIZE + idx * width * 2, y * TILE_SIZE, width * 2, height * 2 };
	graphic->DrawImage(img, dest, 0, 0, width, height, UnitPixel);

	delete img;
}

void Scene::DrawGameUI(Graphics * graphic, int p1Score, int p2Score)
{
	DrawString(graphic, 4, 0, GREEN, "1UP");
	DrawNumber(graphic, 7, 1, WHITE, p1Score);
	DrawString(graphic, 11, 0, RED, "HIGH SCORE");

	int score = Storage::GetInstance().GetTopScore();
	score = (p1Score >= score) ? p1Score : score;
	DrawNumber(graphic, 18, 1, WHITE, score);

	DrawString(graphic, 25, 0, BLUE, "2UP");
	DrawNumber(graphic, 28, 1, WHITE, p2Score);
}

void Scene::DrawHurryUp(Graphics * graphic)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(15, 79, 174), Color(15, 79, 174));

	Image * img = Image::FromFile(L"sprite/char/hurryup.png");
	int width = img->GetWidth();
	int height = img->GetHeight();

	Rect dest = { MAP_WIDTH * TILE_SIZE /2 - width , MAP_HEIGHT * TILE_SIZE / 2 - 1, width * 2, height};
	graphic->DrawImage(img, dest, 0, 0 + 9 * frameCur, width, height / 2, UnitPixel, &imgAttr);

	delete img;
}

bool Scene::DrawReady(Graphics * graphic, int round)
{
	// READY 출력 안 해도 됨 = Enemy 이동 시작
	if (readyCnt == -1) return true;

	// 해야 됨
	readyCnt++;
	if (readyCnt > READY_MAX)
		readyCnt = -1;

	DrawString(graphic, MAP_WIDTH / 2 - 4, MAP_HEIGHT / 2 - 1, WHITE, "ROUND");
	DrawNumber(graphic, MAP_WIDTH / 2 + 3, MAP_HEIGHT / 2 - 1, WHITE, round);
	DrawString(graphic, MAP_WIDTH / 2 - 4, MAP_HEIGHT / 2 + 1, WHITE, "READY !");
	return false;
}


void Scene::DrawLobbyScreen(Graphics * graphic, int credit)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(15, 79, 174), Color(15, 79, 174));

	Image * logo = Image::FromFile(L"sprite/char/logo.png");
	int width = logo->GetWidth();
	int height = logo->GetHeight();
	Rect dest = { 70, 0, width * 2, height * 2 };
	graphic->DrawImage(logo, dest, 0, 0, width, height, UnitPixel, &imgAttr);

	Image * company = Image::FromFile(L"sprite/char/company.png");
	width = company->GetWidth();
	height = company->GetHeight();
	dest = { 175, 310, width * 2, height * 2 };
	graphic->DrawImage(company, dest, 0, 0, width, height, UnitPixel, &imgAttr);

	DrawString(graphic, 5, MAP_HEIGHT - 5, WHITE, "@TAITO CORPORATION 1986");
	DrawString(graphic, 7, MAP_HEIGHT - 3, WHITE, "ALL RIGHTS RESERVED");
	DrawString(graphic, 22, MAP_HEIGHT - 1, WHITE, "CREDIT");
	DrawNumber(graphic, 29, MAP_HEIGHT - 1, WHITE, credit);

	delete logo;
	delete company;
}

void Scene::DrawGameScreen(Graphics * graphic, int round, int p1Life, int p1Score, int p2Score)
{
	if (round != prevRound)
	{
		prevRound = round;
		SetRound(round);
	}

	DrawMap(graphic);
	DrawGameUI(graphic, p1Score, p2Score);
	for (int i = 0; i < p1Life; i++)
		DrawPlayerLife(graphic, P1_LIFE_POS.x, P1_LIFE_POS.y, i);	
}

bool Scene::DrawRoundChange(Graphics * graphic, int round, int p1Life, int p1Score, int p2Score, bool IsPause)
{
	if (round != prevRound)
	{
		prevRound = round;
		SetRound(round);
	}

	if (round == START_ROUND)
	{
		DrawMap(graphic);
		for (int i = 0; i < p1Life; i++)
			DrawPlayerLife(graphic, P1_LIFE_POS.x, P1_LIFE_POS.y, i);
	}
	else
	{
		DrawMap(graphic, true, -roundChangeCnt); // 이전 맵
		DrawMap(graphic, false, (MAP_HEIGHT * TILE_SIZE) - roundChangeCnt - 2 * TILE_SIZE); // 현재 맵
	}
	DrawGameUI(graphic, p1Score, p2Score);

	if (IsPause == true)
		return false;

	roundChangeCnt += ROUND_CHANGE_SPEED;
	if (roundChangeCnt > SCREEN_HEIGHT)
	{
		roundChangeCnt = 0;
		readyCnt = 0;
		return true;
	}	
	return false;
}

void Scene::DrawScoreScreen(Graphics * graphic, int round, int p1Score, const std::string & name)
{
	DrawString(graphic, 6, 4, GREEN, "ENTER 1UP INITIALS !");
	DrawString(graphic, 7, 7, YELLOW, "SCORE ROUND  NAME");
	DrawNumber(graphic, 11, 9, WHITE, p1Score);
	DrawNumber(graphic, 17, 9, WHITE, round);
	DrawString(graphic, 21, 9, WHITE, name);

	DrawString(graphic, 10, 12, WHITE, "SCORE ROUND  NAME");
	DrawString(graphic, 4, 14, WHITE, "1ST");
	DrawString(graphic, 4, 16, WHITE, "2ND");
	DrawString(graphic, 4, 18, WHITE, "3RD");
	DrawString(graphic, 4, 20, WHITE, "4TH");
	DrawString(graphic, 4, 22, WHITE, "5TH");


	for (int i = 0; i < 5; i++)
	{
		DATA cur = Storage::GetInstance().GetTop()[i];
		if (cur.score != 0)
		{
			DrawNumber(graphic, 14, 14 + 2 * i, WHITE, cur.score);
			DrawNumber(graphic, 20, 14 + 2 * i, WHITE, cur.round);
			DrawString(graphic, 24, 14 + 2 * i, WHITE, cur.name);
		}
	}
}

void Scene::DrawGameOverScreen(Graphics * graphic)
{
	DrawString(graphic, MAP_WIDTH / 2 - 5, MAP_HEIGHT / 2 - 1 , WHITE, "GAME OVER");
}



void Scene::DrawPauseScreen(Graphics * graphic, int cursor, int clicked, int vol, bool IsEffectSoundOff)
{
	SolidBrush brush(Color(255, 0, 0, 0));
	Pen pen	(Color(255, 255, 255), 5);

	int width = 18 * TILE_SIZE;
	int height = 14 * TILE_SIZE;
	int x = MAP_WIDTH * TILE_SIZE / 2 - width / 2;
	int y = MAP_HEIGHT * TILE_SIZE / 2 - height / 2;
	
	graphic->FillRectangle(&brush, x, y, width + 5, height + 10);
	graphic->DrawRectangle(&pen, x, y, width + 5, height + 10);

	x /= TILE_SIZE;
	y /= TILE_SIZE;

	int pause_menu_pos_x = x + 1;
	int pause_menu_pos_y[6];
	
	for(int i = 0; i< 6; i++)
		pause_menu_pos_y[i] = y + 2 * (i + 1);

	std::string str[7] = { "CONTINUE" , "RESTART" , "EXIT" , "VOLUME UP" , "VOLUME DOWN", "EFFECT SOUND ON", "EFFECT SOUND OFF" };
	COLOR col;

	for (int i = 0; i < 6; i++)
	{
		col = WHITE;

		if (i == cursor) col = YELLOW;

		if (i == clicked) col = RED;

		if (i == 5) DrawString(graphic, pause_menu_pos_x, pause_menu_pos_y[i], col, str[i + IsEffectSoundOff]);
		else        DrawString(graphic, pause_menu_pos_x, pause_menu_pos_y[i], col, str[i]);
	}

	col = WHITE;
	DrawNumber(graphic, pause_menu_pos_x + 13, pause_menu_pos_y[3] + 1, col, vol);

}

void Scene::UpdateSceneFrame(bool & DrawHurryUp)
{
	delayCnt++;
	
	// DELAY_MAX 만큼 지나고 다음 프레임으로
	if (delayCnt != DELAY_MAX)
		return;
	else if (delayCnt == DELAY_MAX)
		delayCnt = 0;

	// REPEAT_MAX 만큼 반복한다
	if (repeatCnt >= REPEAT_MAX)
	{
		DrawHurryUp = false;
		frameCur = FRAME_MIN;
		repeatCnt = 0;
		return;
	}

	frameCur++;
	if (frameCur >= FRAME_MAX)
	{
		frameCur = FRAME_MIN;
		repeatCnt++;			
	}
}