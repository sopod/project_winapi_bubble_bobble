#include "Node.h"

Node::Node(POINT _pos, int _g, int _h)
{
	pos = _pos;
	g = _g;
	h = _h;
	f = g + h;
}

Node::~Node()
{
}

int GetDistance(POINT from, POINT to)
{
	int min = abs(from.x - to.x) < abs(from.y - to.y) ? abs(from.x - to.x) : abs(from.y - to.y);
	int max = abs(from.x - to.x) > abs(from.y - to.y) ? abs(from.x - to.x) : abs(from.y - to.y);

	return min * 14 + (max - min) * 10;
}




/*
std::vector<POINT> Object::GetPath(POINT from, POINT to, Scene & s)
{
	// 변수들
	const int INF = 9999;
	const int row = 25;
	const int col = 28;
	const int startPos = 2;

	const POINT dir[8] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };  // 8방향 이동 좌표

	int g[row][col];
	int h[row][col];
	POINT prevPos[row][col];

	bool goalIn = false;

	// 초기화 작업
	Node closestNode({ -1, -1 }, INF, INF); // 가장 가까운 노드

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			g[i][j] = INF;
			h[i][j] = INF;
		}
	}
	memset(prevPos, -1, sizeof(prevPos));

	// 첫 시작 위치 큐에 넣기
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
	pq.push(Node(from, 0, GetDistance(from, to)));
	g[from.x][from.y] = 0;
	h[from.x][from.y] = GetDistance(from, to);
	prevPos[from.x][from.y] = from;

	// 큐 작업 시작
	while (pq.empty() == false)
	{
		Node cur = pq.top();
		pq.pop();

		if (cur.pos.x == to.x && cur.pos.y == to.y) // 목적지 도달시 종료
		{
			goalIn = true;
			break;
		}

		if (cur.h < closestNode.h) // 그나마 가까운곳 넣어두기
			closestNode = cur;

		if (cur.g > g[cur.pos.x][cur.pos.y]) // 더 좋은 거리값이 아니면 필요없다
			continue;

		for (int i = 0; i < 8; i++) // 8방향 둘러보기
		{
			POINT curDir = { cur.pos.x + dir[i].x , cur.pos.y + dir[i].y };

			if (curDir.x < 0 || curDir.x >= col) // 범위를 벗어나는 곳 제외
				continue;
			if (curDir.y < 0 || curDir.y >= row)
				continue;
			if (s.GetTile(startPos + curDir.x, startPos + curDir.y) == L'#') // 벽 제외
				continue;

			Node next(curDir, cur.g + GetDistance(cur.pos, curDir), GetDistance(curDir, to));

			if (next.g < g[curDir.x][curDir.y]) // 기존 값보다 더 좋으면 교체하고 큐에 넣기
			{
				g[curDir.x][curDir.y] = next.g;
				h[curDir.x][curDir.y] = next.h;
				prevPos[curDir.x][curDir.y] = cur.pos;
				pq.push(next);
			}
		}
	}

	std::vector<POINT> path;

	// 목표지점에 도달하지 못하면 그나마 가까운 곳을 목표지점으로 삼는다
	if (goalIn == false)
	{
		to.x = closestNode.pos.x;
		to.y = closestNode.pos.y;
		path.push_back({ closestNode.pos.x + startPos, closestNode.pos.y + startPos });
	}

	// 이동경로 도출
	POINT cur = prevPos[to.x][to.y];
	while ((cur.x != from.x || cur.y != from.y) && cur.x != -1)
	{
		path.push_back({cur.x + startPos, cur.y + startPos});
		cur = prevPos[cur.x][cur.y];
	}
	reverse(path.begin(), path.end());

	return path;
}
*/
