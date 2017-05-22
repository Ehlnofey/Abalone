#include <stdio.h>
#include "BasicIA.h"

void print_boardSteel(int board[SIZE][SIZE]) {
	int i, j;

	for (i = SIZE - 1; i >= 0; i--) {
		for (j = 0; j < SIZE; j++) {
			if (board[i][j] == OUT_ZONE) {
				printf("  ");
			}
			else if (board[i][j] == WHITE) {
				printf("2 ");
			}
			else {
				printf("%d ", board[i][j]);
			}
		}

		printf("\n");
	}

	printf("\n\n");
}
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

BasicIA * newBasicIA(int color)
{
	BasicIA *IA = malloc(sizeof(BasicIA));
	IA->color = color;
	IA->t = newTree(NULL,0);
	return IA;
}

void deleteBasicIA(BasicIA * ia)
{
	deepDeleteTree(ia->t);
	free(ia);
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

void makeFloor(TreeList * tl, AbaloneBoard * ab)
{
	TreeList *f = tl;
	while (f != NULL)
	{
		Tree *t = f->t;
		playMove(&t->me, ab);
		buildMovePossibility(t->tl, ab);
		unPlayMove(&t->me, ab);
		f = tl->next;
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
	int res = 0;
	playMove(m, ab);

	if(ab->jumpOver==0)
		res = 0;
	//int dx = m->destx - m->originx, dy = m->desty - m->originy;
	if (ab->board[m->destx + ab->dxj][m->desty + ab->dyj] == OUT_ZONE)
		res = 1;
	if(ab->jumpOver==2)
		if (ab->board[m->destx + 2*ab->dxj][m->desty + 2*ab->dyj] == OUT_ZONE)
			res = 1;

	unPlayMove(m, ab);

	return res;
}

void playMove(Move *m, AbaloneBoard *ab)
{
	printf("Board avant move\n\n");
	print_boardSteel(ab->board);

	isRightCliked(ab, m->destx, m->desty);

	printf("Board apres move\n\n");
	print_boardSteel(ab->board);
}

void unPlayMove(Move *m, AbaloneBoard *ab)
{
	printf("Board avant unmove\n\n");
	print_boardSteel(ab->board);
	ab->board[m->destx][m->desty] = NO_BALL;
	ab->board[m->destx + ab->dxj][m->desty + ab->dyj] = -ab->turn;
	printf("Board apres unmove\n\n");
	print_boardSteel(ab->board);
}
