#pragma once
#include "Draw.h"


void DrawGrid(HDC hdc, RECT rect, int amount)
{
	int dif_x = (rect.right - rect.left) / (amount - 1); // x�� ������
	int dif_y = (rect.bottom - rect.top) / (amount - 1); // y�� ������

	int i, x = 0, y = 0;

	for (i = 0; i < amount; i++)
	{
		// ���μ� 
		MoveToEx(hdc, rect.left + x, rect.top, NULL); // x0, y0 --> x0 +������, y0
		LineTo(hdc, rect.left + x, rect.bottom);      // x0, y1 --> x0 +������, y1
		x += dif_x;

		// ���μ�
		MoveToEx(hdc, rect.left, rect.top + y, NULL); // x0, y0 --> x0, y0 +������
		LineTo(hdc, rect.right, rect.top + y);        // x1, y0 --> x1, y0 +������
		y += dif_y;
	}
}



void DrawRectangle(HDC hdc, int x, int y, int width, int height)
{
	int x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = x + width / 2;
	y1 = y - height / 2;

	x2 = x - width / 2;
	y2 = y - height / 2;

	x3 = x - width / 2;
	y3 = y + height / 2;

	x4 = x + width / 2;
	y4 = y + height / 2;

	Rectangle(hdc, x2, y2, x4, y4);
}

void DrawRectColor(HDC hdc, POINT pos, int size, int r, int g, int b)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(r, g, b));
	HBRUSH hOld = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, pos.x, pos.y, pos.x+size, pos.y+size);

	SelectObject(hdc, hOld);
	DeleteObject(hBrush);
}





void DrawCircle(HDC hdc, float x, float y, int R) // x,y �߽���ǥ, ������
{
	Ellipse(hdc, x - R, y - R, x + R, y + R);
}



void DrawCircleAroundCircle(HDC hdc, float x, float y, int R, int degree) // ū �� x,y �߽���ǥ, ū �� ������, ���� ���� �׸� ����
{
	// ū �� �׸���
	float PI = 3.14159265358979;
	Ellipse(hdc, x - R, y - R, x + R, y + R);

	// ���� ���� ������
	float r = (sin((degree * PI / 180) / 2) * R) / (1 - sin((degree * PI / 180) / 2));
	float deg = 0, rx, ry;

	// degree��ŭ �̵��Ͽ� x, y��ġ�� ���� �� �׸���
	for (int i = 0; i < 360 / degree; i++)
	{
		deg = degree * i;
		rx = x + cos(deg * PI / 180) * (R + r);
		ry = y + sin(deg * PI / 180) * (R + r);
		Ellipse(hdc, x - r, y - r, x + r, y + r);
	}
}




void DrawStar(HDC hdc, float x, float y, int R)	// pt: �߽��� ��ġ, R: �� ���� ���� ������
{
	//DrawCircle(hdc, pt, R);

	float PI = 3.14159265358979;
	const int star = 5;	     // �𼭸��� 5���� ��
	POINT pt[star * 2];   // ���������� ���ؼ� 10���� {x, y}�� ���� ����Ʈ �迭

	float radian = 2 * PI / star;	// �� �𼭸����� �ٸ� �𼭸������� ����
	float r = cos(radian) * R / cos(radian / 2);	// �� ���� ���� ������

	for (int i = 0; i < star; i++)
	{
		// �𼭸�: 0 2 4 6 8 
		// +x  -y: ���������� ���鼭 �׸���.
		pt[i * 2].x = x + sin(radian * i) * R;
		pt[i * 2].y = y - cos(radian * i) * R;
		//DrawCircle(hdc, POINT[i * 2], 5);

		// ������: 1 3 5 7 9
		pt[i * 2 + 1].x = x + sin(radian / 2 + radian * i) * r;
		pt[i * 2 + 1].y = y - cos(radian / 2 + radian * i) * r;
		//DrawCircle(hdc, POINT[i * 2 + 1], 5);
	}

	Polygon(hdc, pt, star * 2);
}


