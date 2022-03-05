// tile_based_map.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "tile_based_map.h"

#include <string>


void DrawRectangle(HDC hdc, float left, float top, float right, float bottom, int r, int g, int b)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(r, g, b));
	HBRUSH hOld = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, left, top, right, bottom);

	SelectObject(hdc, hOld);
	DeleteObject(hBrush);

}


class Map
{
public:
	std::wstring sLevel;

	int nLevelWidth;
	int nLevelHeight;

	int nTileWidth;
	int nTileHeight;

	int nScreenWidth;
	int nScreenHeight;

	Map()
	{
		nLevelWidth = 64;
		nLevelHeight = 16;
		nTileWidth = 20;
		nTileHeight = 20;
		nScreenWidth = nLevelWidth * nTileWidth;
		nScreenHeight = nLevelHeight * nTileHeight;

		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"......................#########........#.#......................";
		sLevel += L"....#...............###................#.#......................";
		sLevel += L"...................####.........................................";
		sLevel += L"###################################################.....########";
		sLevel += L"...................................#.#...............###........";
		sLevel += L"........................############.#............###...........";
		sLevel += L"........................#............#.........###..............";
		sLevel += L"........................#.############......###.................";
		sLevel += L"........................#................###....................";
		sLevel += L"........................#################.......................";
		sLevel += L"................................................................";
	}

	wchar_t GetTile(int x, int y) // x, y 위치의 타일 내용 가져오기
	{
		if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
			return sLevel[y * nLevelWidth + x];
		else
			return L' ';
	}

	void SetTile(int x, int y, wchar_t c)
	{
		if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
			sLevel[y * nLevelWidth + x] = c;
	}

	void DrawMapTile(HDC hdc, int x, int y, int r, int g, int b)
	{
		DrawRectangle(hdc, x * nTileWidth, y * nTileHeight, (x + 1) * nTileWidth, (y + 1) * nTileHeight, r, g, b);
	}
};

Map m;
RECT rt = { 0,0, m.nScreenWidth,m.nScreenHeight };




class Player
{
public:
	float fPlayerPosX;
	float fPlayerPosY;
	float fPlayerVelX;
	float fPlayerVelY;

	Player()
	{
		fPlayerPosX = 0;
		fPlayerPosY = 0;
		fPlayerVelX = 0;
		fPlayerVelY = 0;
	}

	void DrawPlayer(HDC hdc, float fOffsetX, float fOffsetY)
	{
		//DrawRectangle(hdc, (fPlayerPosX - fOffsetX) * m.nTileWidth, (fPlayerPosY - fOffsetY) * m.nTileWidth, (fPlayerPosX - fOffsetX + 1.0f) * m.nTileWidth, (fPlayerPosY - fOffsetY + 1.0f) *m.nTileHeight, 255,0,255);
		DrawRectangle(hdc, fPlayerPosX-m.nTileWidth / 2, fPlayerPosY - m.nTileHeight/2, fPlayerPosX + m.nTileWidth / 2, fPlayerPosY + m.nTileHeight / 2, 255, 0, 255);
	}
};


Player p;








#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TILEBASEDMAP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TILEBASEDMAP));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TILEBASEDMAP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TILEBASEDMAP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.


   AdjustWindowRectEx(&rt, // 원하는 클라이언트 사각형 크기를 기준으로 창 사각형의 필요한 크기를 계산한다.
	   WS_OVERLAPPEDWINDOW,  // 사용하는 윈도우 스타일
	   1, 1); // 메뉴의 존재 여부

   int window_width = rt.right - rt.left;
   int window_height = rt.bottom - rt.top;

   int screen_width = GetSystemMetrics(SM_CXSCREEN); // 화면 가로 너비
   int screen_height = GetSystemMetrics(SM_CYSCREEN); // 화면 세로 높이
   int window_x = (screen_width / 2) - (window_width / 2);
   int window_y = (screen_height / 2) - (window_height / 2);


   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   window_x, window_y, // 모니터에서 윈도우 창의 시작 위치
	   window_width, window_height, // 윈도우의 너비, 높이
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static float fCameraPosX = 0;
	static float fCameraPosY = 0;
	int fElapsedTime = 1;


    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_CREATE:
		fCameraPosX = p.fPlayerPosX;
		fCameraPosY = p.fPlayerPosY;
		break;

	case WM_TIMER:
		break;

	case WM_KEYDOWN:
		{
			p.fPlayerVelX = 0;
			p.fPlayerVelY = 0;
			switch (wParam)
			{
			case VK_UP:
				p.fPlayerVelY = -6;
				break;

			case VK_DOWN:
				p.fPlayerVelY = 6;
				break;

			case VK_LEFT:
				p.fPlayerVelX = -6;
				break;

			case VK_RIGHT:
				p.fPlayerVelX = 6;
				break;
			}

			p.fPlayerPosX += p.fPlayerVelX * fElapsedTime;
			p.fPlayerPosY += p.fPlayerVelY * fElapsedTime;

			InvalidateRect(hWnd, NULL, true);
		}
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...



			fCameraPosX = p.fPlayerPosX;
			fCameraPosY = p.fPlayerPosY;


			int nVisibleTilesX = m.nScreenWidth / m.nTileWidth; // 한 화면에 보이는 타일 수 
			int nVisibleTilesY = m.nScreenHeight / m.nTileHeight;

			float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f; // 화면 가운데 위치 (타일기준)
			float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f; 

			if (fOffsetX < 0) fOffsetX = 0; // 화면이 밖을 벗어나는 것 방지하기 (왼쪽)
			if (fOffsetY < 0) fOffsetY = 0;
			if (fOffsetX > m.nLevelWidth - nVisibleTilesX) fOffsetX = m.nLevelWidth - nVisibleTilesX; // (오른쪽)
			if (fOffsetY > m.nLevelHeight - nVisibleTilesY) fOffsetY = m.nLevelHeight - nVisibleTilesY;


			for (int x = 0; x < nVisibleTilesX; x++)
			{
				for (int y = 0; y < nVisibleTilesY; y++)
				{
					wchar_t sTileID = m.GetTile(x + fOffsetX, y + fOffsetY);

					switch (sTileID)
					{
					case L'.':
						m.DrawMapTile(hdc, x, y, 0, 0, 100);
						break;

					case L'#':
						m.DrawMapTile(hdc, x, y, 100, 0, 100);
						break;

					default:

						break;
					}
				}
			}

			p.DrawPlayer(hdc, fOffsetX, fOffsetY);


            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
