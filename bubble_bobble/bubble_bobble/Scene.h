#pragma once
#include "framework.h"

class Scene
{
private:
	const int TILE_NUMBER[5] = { 35, 17, 192, 33, 66 };
	Image * curTileImg;
	Image * prevTileImg;

	std::vector<Image *> * charImg;
	const int CHAR_GAP = 8;

	std::wstring curMap;
	std::wstring prevMap;
	int prevRound;
	
	const POINT P1_LIFE_POS = {0, 27};

	// 레벨 바뀔 때 화면 전환을 그리기 위한 변수들
	const int SCREEN_HEIGHT = (MAP_HEIGHT - WALLY + 1) * TILE_SIZE;
	const int ROUND_CHANGE_SPEED = 4;
	int roundChangeCnt;

	// 레벨 바뀐 후 READY를 그리기 위한 변수들
	const int READY_MAX = 60;
	int readyCnt;

	// hurry up을 그리기 위한 변수들
	const int FRAME_MAX = 2;
	const int FRAME_MIN = 0;
	int frameCur;

	const int DELAY_MAX = 7;
	int delayCnt;

	const int REPEAT_MAX = 4;
	int repeatCnt;

	static Scene * instance;

	Scene();
	Scene(const Scene & ref);
	~Scene();
	
public:
	static Scene & GetInstance()
	{
		static Scene s;
		return s;
	}

	void InitScene(int round);

	void SetRound(int round);

	POINT GetTileWithPos(fPoint pos);
	wchar_t GetTileWithIdx(int x, int y);
	wchar_t GetTileWithIdxAndMap(int x, int y, std::wstring * curMap);
	void SetTileWithIdx(int x, int y, wchar_t c);

	bool PosVsTile(float x, float y);
	fPoint PosVsTileWhere(float x, float y);
	
	int GetCharPos(char c);
	void DrawChar(Graphics * graphic, int x, int y, COLOR color, char c);
	void DrawString(Graphics * graphic, int x, int y, COLOR color, const std::string & str);
	void DrawNumber(Graphics * graphic, int x, int y, COLOR color, int & n);

	void DrawTile(Graphics * graphic, bool isPrevRound, int x, int y);
	void DrawMap(Graphics * graphic);
	void DrawMap(Graphics * graphic, bool isPrevRound, int startY);

	void DrawPlayerLife(Graphics *graphic, int x, int y, int idx);
	void DrawGameUI(Graphics * graphic, int p1Score, int p2Score);
	void DrawHurryUp(Graphics *graphic);
	bool DrawReady(Graphics *graphic, int round);

	void DrawLobbyScreen(Graphics * graphic, int credit);
	void DrawGameScreen(Graphics * graphic, int round, int p1Life, int p1Score, int p2Score);
	bool DrawRoundChange(Graphics * graphic, int round, int p1Life, int p1Score, int p2Score, bool isPause);
	void DrawScoreScreen(Graphics * graphic, int round, int p1Score, const std::string & str);
	void DrawGameOverScreen(Graphics * graphic);
	void DrawPauseScreen(Graphics * graphic, int cursor, int clicked, int vol, bool isEffectSoundOff);

	void UpdateSceneFrame(bool & isHurryUp);
};

