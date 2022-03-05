#pragma once
#include "framework.h"

class Node
{
public:
	POINT pos;
	int g;
	int h;
	int f;

	Node(POINT _pos, int _g, int _h);
	~Node();

	Node & operator=(const Node & n)
	{
		pos = n.pos;
		g = n.g;
		h = n.h;
		f = n.f;
		return *this;
	}

	friend bool operator>(Node a, Node b)
	{
		if (a.f == b.f)
			return a.g > b.g;
		else
			return a.f > b.f;
	}
};

int GetDistance(POINT from, POINT to);

