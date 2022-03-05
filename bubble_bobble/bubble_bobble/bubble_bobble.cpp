#include "framework.h"
#include "bubble_bobble.h"




#include "GameCenter.h"

ULONG_PTR g_GdiToken;

LARGE_INTEGER g_tSecond; // 초당 클록 수 
LARGE_INTEGER g_tTime;   // 이전 클록 수
float g_fDeltaTime;      // 델타 타임 = 초당 클럭 수 

void Run();





#define MAX_LOADSTRING 100

HINSTANCE hInst;                    
WCHAR szTitle[MAX_LOADSTRING];      
WCHAR szWindowClass[MAX_LOADSTRING];

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
    
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BUBBLEBOBBLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BUBBLEBOBBLE));

    MSG msg;


	QueryPerformanceFrequency(&g_tSecond); // 초당 클록 수 가져오기
	QueryPerformanceCounter(&g_tTime);     // 이전 클록 수 가져오기

	while (true) // 새로 만든 메시지 루프 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Run();
		}
	}
    return (int) msg.wParam;
}

void Run()
{	
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	DWORD durTime = newTime - oldTime;
	if (durTime < 30)
		return;
	oldTime = newTime;
	

	
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime); // 현재 클록 수 가져오기

	// Run함수의 이전 호출과 이번 호출 사이의 클럭수 = (현재 클록 수 - 이전 클록 수) 
	// Run함수의 1초 당 클럭 수 = (현재 클록 수 - 이전 클록 수) / 초당 클록 수
	g_fDeltaTime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tSecond.QuadPart;
	g_tTime = tTime; // 이전 클록 수 = 현재 클록 수 (다음 계산을 위해)

	// 속도 = 플레이어의 초당 이동속도 * Run함수의 1초 당 클럭 수
	float fSpeed = 130 * g_fDeltaTime; // 130

	GameCenter::GetInstance().Update(fSpeed);
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BUBBLEBOBBLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BUBBLEBOBBLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   RECT rt = GameCenter::GetInstance().GetWindowSize();

   AdjustWindowRectEx(&rt,    // 원하는 클라이언트 사각형 크기를 기준으로 창 사각형의 필요한 크기를 계산한다.
	   WS_OVERLAPPEDWINDOW,   // 사용하는 윈도우 스타일
	   0, 0); // 메뉴의 존재 여부

   int window_width = rt.right - rt.left ; 
   int window_height = rt.bottom - rt.top + 20;

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



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiToken, &gpsi, NULL);

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
		{
			GameCenter::GetInstance().Init();
			srand(time(NULL));

			SetTimer(hWnd, 1, 16, NULL);  // 화면 그려주기 , 프레임 업데이트 16	
		}
		break;

	case WM_TIMER:
		{
			switch (wParam)
			{
			case 1:
				InvalidateRect(hWnd, NULL, false);
				break;
			}
		}
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			if (GameCenter::GetInstance().GetIsExitSelected() == true)
				SendMessage(hWnd, WM_CLOSE, 0, 0);

			GameCenter::GetInstance().Draw(hdc);
			GameCenter::GetInstance().UpdateFrame();

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);

		GdiplusShutdown(g_GdiToken);

        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
