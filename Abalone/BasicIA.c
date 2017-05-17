#include "BasicIA.h"

void extendSelection(TreeList *tl, AbaloneBoard * ab, int i, int j, int di, int dj)
{
	isLeftCliked(ab, i + di, j + dj);
	if (ab->selectedBalls == 2)
	{
		addMove(tl, ab, i, j);
		isLeftCliked(ab, i + 2 * di, j + 2 * dj);
		if (ab->selectedBalls == 3)
			addMove(tl, ab, i, j);
		isLeftCliked(ab, i + 2 * di, j + 2 * dj);
		isLeftCliked(ab, i + di, j + dj);
	}
}

void buildMovePossibility(TreeList *tl, AbaloneBoard * ab)
{
	int i = 0, j = 0;
	for (i = 0;i < SIZE;i++)
	{
		for (j = 0;j < SIZE;j++)
		{
			if (ab->board[i][j] == ab->turn)
			{
				isLeftCliked(ab, i, j);
				addMove(tl, ab, i, j);
				extendSelection(tl, ab, i, j, 1, 0);
				extendSelection(tl, ab, i, j, 0, 1);
				extendSelection(tl, ab, i, j, 1, 1);
				isLeftCliked(ab, i, j);
			}
		}
	}
}

void canAddMoveTest(TreeList * tl, AbaloneBoard * ab, int x, int y, int dx, int dy)
{
	Move m;
	m.originx = x;
	m.originy = y;
	m.destx = x + dx;
	m.desty = y + dy;

	if (canMove(ab, x + dx, y + dy))
		appendTreeList(tl, newTree(&m, workOutWeight(ab, &m)));
}

void addMove(TreeList * tl, AbaloneBoard * ab, int x, int y)
{
	canAddMoveTest(tl, ab, x, y, 1, 0);
	canAddMoveTest(tl, ab, x, y, 0, 1);
	canAddMoveTest(tl, ab, x, y, 1, 1);
	canAddMoveTest(tl, ab, x, y, -1, 0);
	canAddMoveTest(tl, ab, x, y, 0, -1);
	canAddMoveTest(tl, ab, x, y, -1, -1);
}

int workOutWeight(AbaloneBoard * ab, Move * m)
{
	if(ab->jumpOver==0)
		return 0;
	//int dx = m->destx - m->originx, dy = m->desty - m->originy;
	if (ab->board[m->destx + ab->dxj][m->desty + ab->dyj] == OUT_ZONE)
		return 1;
	if(ab->jumpOver==2)
		if (ab->board[m->destx + 2*ab->dxj][m->desty + 2*ab->dyj] == OUT_ZONE)
			return 1;

	return 0;
}
