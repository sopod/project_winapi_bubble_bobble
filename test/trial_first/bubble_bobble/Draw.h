#pragma once
#include "framework.h"
#include <cmath>

void DrawGrid(HDC hdc, RECT rect, int amount);

void DrawRectangle(HDC hdc, int x, int y, int width, int height);

void DrawRectColor(HDC hdc, POINT pos, int size, int r, int g, int b);

void DrawCircle(HDC hdc, float x, float y, int R);

void DrawCircleAroundCircle(HDC hdc, float x, float y, int R, int degree);

void DrawStar(HDC hdc, float x, float y, int R);