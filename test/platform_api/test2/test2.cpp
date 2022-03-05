// test2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "test2.h"

#include <iostream>
	#include <vector> 
	#include <algorithm>

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
    LoadStringW(hInstance, IDC_TEST2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST2));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TEST2);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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


class FPOINT
{
public:
	float x, y;

	FPOINT Norm()
	{
		float r = 1 / std::sqrt(x * x + y * y);
		return { r * x, r * y };
	}

	FPOINT Abs()
	{
		return { std::abs(x), std::abs(y) };
	}

	const FPOINT operator+(const FPOINT & p) const
	{
		return { x + p.x , y + p.y };
	}

	const FPOINT operator-(const FPOINT & p) const
	{
		return { x - p.x , y - p.y };
	}

	FPOINT operator*(const FPOINT & p) const
	{
		return { x * p.x , y * p.y };
	}

	FPOINT operator/(const FPOINT & p) const
	{
		return { x / p.x , y / p.y };
	}

	FPOINT operator*(float f) const
	{
		return { f * x , f * y };
	}

	friend FPOINT operator*(float f, const FPOINT & p)
	{
		return { f * p.x , f * p.y };
	}

	FPOINT operator/(float f) const
	{
		return { x / f, y / f };
	}
	
	friend FPOINT operator/(float f, const FPOINT & p)
	{
		return { f / p.x , f / p.y };
	}

	FPOINT operator+=(const FPOINT & p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}
};


struct rect
{
	FPOINT pos;  // 좌상단 위치
	FPOINT size; // 가로 세로 길이
	FPOINT vel;  // 움직이는 방향 * 스피드 = 이동 값
};


void DrawRect(HDC hdc, const rect &r, const int color)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(color, 0, 255));
	HPEN oPen = (HPEN)SelectObject(hdc, hPen);

	Rectangle(hdc, r.pos.x, r.pos.y, r.pos.x + r.size.x, r.pos.y + r.size.y);

	SelectObject(hdc, oPen);
	DeleteObject(hPen);
}

void DrawCircle(HDC hdc, const FPOINT &r, const int color)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(color, 0, 0));
	HBRUSH oBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Ellipse(hdc, r.x, r.y, r.x + 5, r.y + 5);

	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);
}

void DrawLine(HDC hdc, const FPOINT &from, const FPOINT &to)
{
	MoveToEx(hdc, from.x, from.y, NULL);
	LineTo(hdc, to.x, to.y);
}

bool PointVsRect(const FPOINT & p, const rect & r)
{
	return (p.x >= r.pos.x && p.y >= r.pos.y && p.x < r.pos.x + r.size.x && p.y < r.pos.y + r.size.y);
}

bool RectVsRect(const rect & r1, const rect & r2)
{
	return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x &&
		r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y);
}

bool RayVsRect(const FPOINT& ray_origin, const FPOINT& ray_dir, const rect & target, 
	FPOINT& contact_point, FPOINT& contact_normal, float& t_hit_near)
{
	contact_normal = { 0,0 };
	contact_point = { 0,0 };

	// Cache division
	FPOINT invdir = 1.0f / ray_dir; // ray_dir으로 나누기

	// Calculate intersections with rectangle bounding axes
	FPOINT t_near = (target.pos - ray_origin) * invdir;
	FPOINT t_far = (target.pos + target.size - ray_origin) * invdir;

	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false; // 0으로 나눴을 때 = ray_dir가 0일 때 
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

	// Sort distances
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	// Early rejection		
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// Closest 'time' will be the first contact
	t_hit_near = max(t_near.x, t_near.y);

	// Furthest 'time' is contact on opposite side of target
	float t_hit_far = min(t_far.x, t_far.y);

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// Contact point of collision from parametric line equation
	contact_point = ray_origin + t_hit_near * ray_dir;

	if (t_near.x > t_near.y)
		if (invdir.x < 0)
			contact_normal = { 1, 0 };
		else
			contact_normal = { -1, 0 };
	else if (t_near.x < t_near.y)
		if (invdir.y < 0)
			contact_normal = { 0, 1 };
		else
			contact_normal = { 0, -1 };

	// Note if t_near == t_far, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;
}

bool DynamicRectVsRect(const rect & in, const rect & target, 
	FPOINT & contact_point, FPOINT & contact_normal, float & contact_time, float fElapsedTime)
{
	if (in.vel.x == 0 && in.vel.y == 0)
		return false;

	rect expanded_target;
	expanded_target.pos = target.pos - in.size / 2;
	expanded_target.size = target.size + in.size;

	if (RayVsRect(in.pos + in.size/2, in.vel * fElapsedTime, expanded_target, contact_point, contact_normal, contact_time))
	{
		if (contact_time >= 0.0f && contact_time < 1.0f)
			return true;
	}

	return false;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static FPOINT gravity = { 0, 2 };

	static FPOINT vMouse;        // 플레이어의 키보드, 마우스의 조작
	static FPOINT ray_direction; // 그로 인해 생기는 방향성

	static float fElapsedTime;
	static FPOINT cp, cn;
	static float ct;

	static std::vector<rect> vRects;
	static std::vector<std::pair<int, float>> z;

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
	{
		vRects.push_back({ {300, 70.0f}, {30.0f, 30.0f} }); // 플레이어

		vRects.push_back({ {300, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {350, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {400, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {450, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {500, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {550, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {600, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {650, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {700, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {750, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {800, 300}, {50.0f, 50.0f} });
		vRects.push_back({ {850, 300}, {50.0f, 50.0f} });

		vRects.push_back({ {900, 150}, {50.0f, 50.0f} });
		vRects.push_back({ {950, 150}, {50.0f, 50.0f} });
		vRects.push_back({ {1000, 150}, {50.0f, 50.0f} });
		vRects.push_back({ {1050, 150}, {50.0f, 50.0f} });

		fElapsedTime = 1;

		SetTimer(hWnd, 1, 100, NULL); // 10 FPS
	}
	break;

	case WM_TIMER:
		{
			switch (wParam)
			{
			case 1:
				
				// 충돌 처리
				for (int i = 1; i < vRects.size(); i++)
				{
					if (DynamicRectVsRect(vRects[0], vRects[i], cp, cn, ct, fElapsedTime))
					{
						z.push_back({ i, ct });
					}
				}

				std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float> & b)
				{
					return a.second < b.second;
				});

				for(auto j : z)
					if (DynamicRectVsRect(vRects[0], vRects[j.first], cp, cn, ct, fElapsedTime))
					{
						vRects[0].vel += cn * vRects[0].vel.Abs() * (1 - ct);
					}

				// 업데이트
				vRects[0].pos += vRects[0].vel * fElapsedTime;

				vRects[0].vel.y += gravity.y * fElapsedTime;

				break;
			}
			// 그리기
			InvalidateRect(hWnd, NULL, true);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			/*
			vMouse.x = LOWORD(lParam);
			vMouse.y = HIWORD(lParam);
			ray_direction = vMouse - vRects[0].pos;  // 플레이어의 방향성 계산	
			vRects[0].vel = ray_direction.Norm() * 10 * fElapsedTime; // 속도 증가는 +=

			InvalidateRect(hWnd, NULL, true);
			*/
		}
		break;

	case WM_KEYDOWN:
		{
			vRects[0].vel = { 0, 0 };
			switch (wParam)
			{
			case VK_LEFT:
				vRects[0].vel.x = -20;
				break;

			case VK_RIGHT:
				vRects[0].vel.x = 20;
				break;

			case VK_UP:
				break;

			case VK_DOWN:
				break;

			case VK_SPACE: // 점프
				vRects[0].vel.y = -20;
				break;
			}
		}
		break;


    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
								   				
			
			for (const auto & r : vRects)
				DrawRect(hdc, r, 255);
			

			
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
