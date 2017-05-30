#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "AbaloneBoard.h"
#include "minimax.h"
//#include "BasicIA.h"

int defaultBoard[SIZE][SIZE] = {
	{ BLACK, BLACK, BLACK, BLACK, BLACK, OUT_ZONE, OUT_ZONE, OUT_ZONE, OUT_ZONE },
	{ BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, OUT_ZONE, OUT_ZONE, OUT_ZONE },
	{ NO_BALL, NO_BALL, BLACK, BLACK, BLACK, NO_BALL, NO_BALL, OUT_ZONE, OUT_ZONE },
	{ NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, OUT_ZONE },
	{ NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL },
	{ OUT_ZONE, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL },
	{ OUT_ZONE, OUT_ZONE, NO_BALL, NO_BALL, WHITE, WHITE, WHITE, NO_BALL, NO_BALL },
	{ OUT_ZONE, OUT_ZONE, OUT_ZONE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE },
	{ OUT_ZONE, OUT_ZONE, OUT_ZONE, OUT_ZONE, WHITE, WHITE, WHITE, WHITE, WHITE }
};

int belgianDaisyBoard[SIZE][SIZE] = {
	{ BLACK, BLACK, NO_BALL, WHITE, WHITE, OUT_ZONE, OUT_ZONE, OUT_ZONE, OUT_ZONE },
	{ BLACK, BLACK, BLACK, WHITE, WHITE, WHITE, OUT_ZONE, OUT_ZONE, OUT_ZONE },
	{ BLACK, BLACK, NO_BALL, NO_BALL, WHITE, WHITE, NO_BALL, OUT_ZONE, OUT_ZONE },
	{ NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, OUT_ZONE },
	{ NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL },
	{ OUT_ZONE, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL, NO_BALL },
	{ OUT_ZONE, OUT_ZONE, NO_BALL, WHITE, WHITE, NO_BALL, BLACK, BLACK, NO_BALL },
	{ OUT_ZONE, OUT_ZONE, OUT_ZONE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK },
	{ OUT_ZONE, OUT_ZONE, OUT_ZONE, OUT_ZONE, WHITE, WHITE, NO_BALL, BLACK, BLACK }
};

int handleClik(void* handler, Event *e);

AbaloneBoard * newAbaloneBoard(EventManager *em, SDL_Renderer *ren, TextureManager *tm, int blackBallCount, int whiteBallCount)
{
	AbaloneBoard *ab = newTheoricalAbaloneBoard(blackBallCount, whiteBallCount);
	int i = 0, j = 0;

	ab->black = malloc(sizeof(Drawable)*blackBallCount);
	ab->white = malloc(sizeof(Drawable)*whiteBallCount);
	ab->blackDeadBalls = 0;
	ab->whiteDeadBalls = 0;

	ab->boardDrawable = getDrawable(tm, ren, "./Image/AbaloneBoard.png", 0, 0);
	for (i = 0;i < blackBallCount;i++)
		ab->black[i] = getDrawable(tm, ren, "./Image/blackBall.png", 0, 0);
	
	for (i = 0;i < whiteBallCount;i++)
		ab->white[i] = getDrawable(tm, ren, "./Image/whiteBall.png", 0, 0);

	getDrawable(tm, ren, "./Image/selectedBallBlack.png", 0, 0);
	getDrawable(tm, ren, "./Image/selectedBallWhite.png", 0, 0);

	addCallback(em, handleClik, SDL_EVENT, ab);

	return ab;
}

AbaloneBoard * newTheoricalAbaloneBoard(int blackBallCount, int whiteBallCount)
{
	AbaloneBoard *ab = malloc(sizeof(AbaloneBoard));
	int i = 0, j = 0;

	ab->blackBallsCount = blackBallCount;
	ab->whiteBallsCount = whiteBallCount;
	ab->turn = BLACK;
	ab->selectedBalls = 0;

	for (i = 0;i < SIZE;i++)
		for (j = 0;j < SIZE;j++)
			ab->board[i][j] = OUT_ZONE;

	return ab;
}

int someoneWin(AbaloneBoard * ab)
{
	if (ab->blackDeadBalls >= 6)
		return WHITE;

	if (ab->whiteDeadBalls >= 6)
		return BLACK;

	return NO_BALL;
}

void copyTheoricalAbaloneBoard(AbaloneBoard * dst, AbaloneBoard * src)
{
	int i, j;
	dst->blackBallsCount = src->blackBallsCount;
	dst->whiteBallsCount = src->whiteBallsCount;
	dst->turn = src->turn;
	dst->selectedBalls = src->selectedBalls;
	for (i = 0; i < 3;i++)
		dst->x[i] = src->x[i];
	for (i = 0; i < 3; i++)
		dst->y[i] = src->y[i];
	dst->jumpOver = src->jumpOver;
	dst->dxj = src->dxj;
	dst->dyj = src->dyj;
	dst->broadSizeMove = src->broadSizeMove;
	dst->blackDeadBalls = src->blackDeadBalls;
	dst->whiteDeadBalls = src->whiteDeadBalls;

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			dst->board[i][j] = src->board[i][j];
}

void setDefaultConf(AbaloneBoard * ab)
{
	int i, j;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			ab->board[i][j] = defaultBoard[i][j];
		}
	}
}

void drawBoard(AbaloneBoard * ab, EventManager * em)
{
	Event e;
	int i = 0, j = 0, bc = 0, wc = 0;
	e.code = DRAW;
	e.haveToBeDelete = 0;
	e.data = &ab->boardDrawable;

	pushEvent(e);

	for (i = 0;i < SIZE;i++)
	{
		for (j = 0;j < SIZE;j++)
		{
			if (ab->board[i][j] == BLACK || ab->board[i][j] == SELECTED_BLACK)
			{
				e.data = &ab->black[bc];
				bc++;
				pushEvent(e);
			}
			else if (ab->board[i][j] == WHITE || ab->board[i][j] == SELECTED_WHITE)
			{
				e.data = &ab->white[wc];
				wc++;
				pushEvent(e);
			}
		}
	}

	ab->blackDeadBalls = ab->blackBallsCount - bc;
	ab->whiteDeadBalls = ab->whiteBallsCount - wc;

}

void setConf(AbaloneBoard * ab,int (*board)[9][9])
{
	int i = 0, j = 0;
	for (i = 0;i < SIZE;i++)
		for (j = 0;j < SIZE;j++)
			ab->board[i][j]=(*board)[i][j];
}

void deleteAdaloneBoard(AbaloneBoard * ab)
{
	free(ab->black);
	free(ab->white);
	free(ab);
}

void convertCoord(int boardLetter, int boardNumber, int * screenX, int * screenY)
{
	if (boardLetter > 4)
		boardNumber -= boardLetter - 4;
	boardLetter -= 4;
	boardLetter *= -1;
	*screenX = (int)(109+abs(boardLetter)*cos(60./180.*M_PI)*66+boardNumber*66);
	if (boardLetter == 0)
		*screenY = 293;
	else
		*screenY = 293 + boardLetter * 57;
		

}

void screenToBoard(int screenX, int screenY, int *boardL, int *boardN)
{
	*boardL = (int)(floor((screenY - 293.) / 57.)); 
	*boardN = (int)((screenX - (109 + abs(*boardL)*cos(60. / 180.*M_PI) * 66)) / 66);
	*boardL *= -1;
	*boardL += 4;
	if (*boardL > 4)
		*boardN += *boardL - 4;
}

void setDrawableCoord(AbaloneBoard * ab, TextureManager *tm)
{
	int i = 0, j = 0, bc = 0, wc = 0;

	for (i = 0;i < SIZE;i++)
	{
		for (j = 0;j < SIZE;j++)
		{
			if (ab->board[i][j] == BLACK || ab->board[i][j] == SELECTED_BLACK)
			{
				if (bc >= ab->blackBallsCount)
				{
					printf("Not enough black balls");
					assert(bc < ab->blackBallsCount);
				}

				convertCoord(i, j, &ab->black[bc].dst.x, &ab->black[bc].dst.y);
				if (ab->board[i][j] == SELECTED_BLACK)
					ab->black[bc].tex = getTexture(tm, "./Image/selectedBallBlack.png");
				else
					ab->black[bc].tex = getTexture(tm, "./Image/blackBall.png");

				bc++;
			}
			else if (ab->board[i][j] == WHITE || ab->board[i][j] == SELECTED_WHITE)
			{
				if (wc >= ab->whiteBallsCount)
				{
					printf("Not enough white balls");
					assert(wc < ab->whiteBallsCount);
				}

				convertCoord(i, j, &ab->white[wc].dst.x, &ab->white[wc].dst.y);
				if(ab->board[i][j]==SELECTED_WHITE)
					ab->white[wc].tex = getTexture(tm, "./Image/selectedBallWhite.png");
				else
					ab->white[wc].tex = getTexture(tm, "./Image/whiteBall.png");
				wc++;
			}
		}
	}
}

void coordConv(int x[3], int y[3], int r[3], double phi[3], int size)
{
	int i = 0;
	for (i = 0;i < size;i++)
	{
		r[i] = x[i] * x[i] + y[i] * y[i];
		phi[i] = atan((double)(y[i]) / (double)(x[i]));
	}
}

int isCorrectForXYAlign(int x[3], int y[3], int size)
{
	if (size == 1)
		return 1;
	if (size == 2)
		return abs(x[0] - x[1]) == 1 && abs(y[0] - y[1]) == 1;
	if (size == 3)
		return (abs(x[0] - x[1]) == 2 && abs(y[0] - y[1]) == 2) || (abs(x[0] - x[2]) == 2 && abs(y[0] - y[2]) == 2)
		|| (abs(x[2] - x[1]) == 2 && abs(y[2] - y[1]) == 2);
	return 0;
}

int isCorrectForXAlign(int x[3], int y[3], int size)
{
	if (size == 1)
		return 1;
	if (size == 2)
		return abs(x[0] - x[1]) == 1 && abs(y[0] - y[1]) == 0;
	if (size == 3)
		return (abs(x[0] - x[1]) == 2 && abs(y[0] - y[1]) == 0) || (abs(x[0] - x[2]) == 2 && abs(y[0] - y[2]) == 0)
		|| (abs(x[2] - x[1]) == 2 && abs(y[2] - y[1]) == 0);
	return 0;
}

int handleClik(void* handler, Event * e)
{
	int i = 0, j = 0, x = 0, y = 0, wc = 0, bc = 0;
	SDL_Event *evt = (SDL_Event*)(e->data);
	AbaloneBoard *ab = (AbaloneBoard*) handler;

	if (evt->type == SDL_MOUSEBUTTONUP&&evt->button.button==SDL_BUTTON_LEFT)
	{
		screenToBoard(evt->button.x, evt->button.y, &x, &y);

		if (x >= 0 && x < SIZE && y >= 0 && y < SIZE)
			isLeftCliked(ab,x,y);
	}
	else if (evt->type == SDL_MOUSEBUTTONUP && evt->button.button == SDL_BUTTON_RIGHT)
	{
		screenToBoard(evt->button.x, evt->button.y, &x, &y);

		if (x >= 0 && x < SIZE && y >= 0 && y < SIZE)
			isRightCliked(ab,x, y);
	}
	else if (evt->type == SDL_MOUSEBUTTONUP && evt->button.button == SDL_BUTTON_MIDDLE)
	{
		screenToBoard(evt->button.x, evt->button.y, &x, &y);
		printf("Case %c%d cliked\n",65+x,y+1);
	}

	return 0;
}

int getOpponentBallsCount(AbaloneBoard *ab, int ox, int oy, int dx, int dy, int color)
{
	int c = 0;

	if (color == BLACK)
		color = WHITE;
	else
		color = BLACK;

	if (ab->board[ox][oy] == color)
	{
		c++;
		if (ox + dx >= 0 && ox + dx < SIZE && oy + dy >= 0 && oy + dy < SIZE && ab->board[ox + dx][oy + dy] == color)
		{
			c++;
			if (ox + 2 * dx >= 0 && ox + 2 * dx < SIZE && oy + 2 * dy >= 0 && oy + 2 * dy < SIZE &&ab->board[ox + 2 * dx][oy + 2 * dy] == color)
				c++;
		}
	}

	return c;
}

void move(AbaloneBoard *ab, int ox, int oy, int destx, int desty)
{
	int i = 0, j = 0;

	for (i = 0; i < ab->selectedBalls; i++)
		ab->board[ab->x[i]][ab->y[i]] = ab->turn;

	ab->board[ox][oy] = NO_BALL;
	
	if (ab->jumpOver > 0)
	{
		if (destx + ab->dxj >= SIZE || desty + ab->dyj >= SIZE)
		{
			if (-ab->turn == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else if (ab->board[destx + ab->dxj][desty + ab->dyj] == OUT_ZONE)
		{
			if (-ab->turn == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else if(ab->jumpOver == 1)
			ab->board[destx + ab->dxj][desty + ab->dyj] = -ab->turn;
		else if (destx + 2 * ab->dxj >= SIZE || desty + 2 * ab->dyj >= SIZE)
		{
			if (-ab->turn == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else if (ab->board[destx + 2*ab->dxj][desty + 2*ab->dyj] == OUT_ZONE)
		{
			if (-ab->turn == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else
			ab->board[destx + 2*ab->dxj][desty + 2*ab->dyj] = -ab->turn;
	}

	ab->board[destx][desty] = ab->turn;
}

int euclidianDist(int x1, int y1, int x2, int y2)
{
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}
	
void nearest(int x1, int y1, int x2, int y2, int x, int y, int *fx, int *fy, int *rx, int *ry)
{
	if (euclidianDist(x1, y1, x, y) < euclidianDist(x2, y2, x, y))
	{
		*fx = x1;
		*fy = y1;
		*rx = x2;
		*ry = y2;
	}
	else if (euclidianDist(x1, y1, x, y) == euclidianDist(x2, y2, x, y))
	{

	}
	else
	{
		*fx = x2;
		*fy = y2;
		*rx = x1;
		*ry = y1;
	}
}

int sign(int x)
{
	if (x < 0)
		return -1;
	if (x > 0)
		return 1;
	return 0;
}

int canBroadMove(AbaloneBoard *ab, int x, int y)
{
	int i = 0, j = 0, allow = 0;
	ab->broadSizeMove = 1;

	for (i = 0;i < ab->selectedBalls && !allow;i++)
	{
		int e = euclidianDist(x, y, ab->x[i], ab->y[i]);
		if (e == 1 || e == 2)
		{
			int px = ab->x[i], py = ab->y[i];
			allow = 1;
			ab->x[i] = ab->x[0];
			ab->y[i] = ab->y[0];
			ab->x[0] = px;
			ab->y[0] = py;
		}

		ab->dxj = x - ab->x[0];
		ab->dyj = y - ab->y[0];

		for (j = 0;j < ab->selectedBalls && allow;j++)
			if (ab->x[j] + ab->dxj < SIZE && ab->x[j] + ab->dxj >= 0 && ab->y[j] + ab->dyj < SIZE && ab->y[j] + ab->dyj >= 0)
				allow = allow && (ab->board[ab->x[j] + ab->dxj][ab->y[j] + ab->dyj] == NO_BALL);
	}
	

	return allow;
}

int canMove(AbaloneBoard *ab,int x, int y)
{
	int allow = 0;
	ab->jumpOver = 0;
	ab->broadSizeMove = 0;

	if (ab->selectedBalls != 0)
	{
		int i = 0, j = 0, k= 0;
		int dx, dy;
		double a;
		double b;
		int isAlign = 1;

		if (ab->selectedBalls > 1)
		{
			if (ab->x[0] != ab->x[1])
			{
				a = (ab->y[0] - ab->y[1]) / (ab->x[0] - ab->x[1]);
				b = ab->y[0] - a*ab->x[0];
				if (y != a*x + b)
					isAlign = 0;
			}
			else
			{
				a = (ab->x[0] - ab->x[1]) / (ab->y[0] - ab->y[1]);
				b = ab->x[0] - a*ab->y[0];
				if (x != a*y + b)
					isAlign = 0;
			}

			if (isAlign)
			{
				for (j = 0;j < ab->selectedBalls - 1;j++)
					for (i = 0;i < ab->selectedBalls - 1;i++)
						nearest(ab->x[i], ab->y[i], ab->x[i + 1], ab->y[i + 1], x, y, &ab->x[i], &ab->y[i], &ab->x[i + 1], &ab->y[i + 1]);
			}
			else if(ab->selectedBalls>1)
				return canBroadMove(ab, x, y);
		}

		dx = x - ab->x[0];
		dy = y - ab->y[0];

		ab->dxj = dx;
		ab->dyj = dy;

		if (abs(dx) > 1 || abs(dy > 1))
			return 0;

		if (ab->turn + ab->board[x][y] == 0)
		{
			if (ab->selectedBalls == 1 || isAlign == 0)
				return 0;

			ab->jumpOver++;

			if (ab->x[0] + 2*dx >= SIZE || ab->y[0] + 2*dy >= SIZE || ab->x[0] + 2 * dx < 0 || ab->y[0] + 2 * dy < 0)
				return 1;

			if (ab->board[ab->x[0] + 2*dx][ab->y[0] + 2*dy] == NO_BALL || ab->board[ab->x[0] + 2 * dx][ab->y[0] + 2 * dy] == OUT_ZONE)
				return 1;
			
			if (ab->board[ab->x[0] + 2*dx][ab->y[0] + 2*dy] == -ab->turn && ab->selectedBalls == 3)
			{
				ab->jumpOver++;

				if (ab->x[0] + 3*dx >= SIZE || ab->y[0] + 3*dy >= SIZE || ab->x[0] + 3 * dx < 0 || ab->y[0] + 3 * dy < 0)
					return 1;

				return (ab->board[ab->x[0] + 3 * dx][ab->y[0] + 3 * dy] == NO_BALL) || (ab->board[ab->x[0] + 3 * dx][ab->y[0] + 3 * dy] == OUT_ZONE);
			}

			ab->jumpOver = 0;
			return 0;
		}
		else if (ab->board[x][y] == 0)
		{
			if (isAlign)
			{
				if (ab->selectedBalls == 1)
					allow = (abs(ab->x[0] - x) <= 1 && abs(y - ab->y[0]) <= 1) && (x != ab->x[0] || y != ab->y[0]) && (abs(dx)!=abs(dy)||sign(dx)==sign(dy));
				else 
				{
					if (ab->x[0] != ab->x[1] && ab->y[0] != ab->y[1])
						allow = (abs(ab->x[0] - x) == 1 && abs(y - ab->y[0]) == 1);
					else if (ab->x[0] != ab->x[1])
						allow = (abs(ab->x[0] - x) == 1 && abs(y - ab->y[0]) == 0);
					else
						allow = (abs(ab->x[0] - x) == 0 && abs(y - ab->y[0]) == 1);
				}
			}
			else
			{
				int i;
				for (i = 0; i < ab->selectedBalls; i++)
				{
					if (ab->x[i] + dx < 0 || ab->x[i] + dx >= SIZE || ab->y[i] + dy < 0 || ab->y[i] + dy >= SIZE)
						return 0;
					if (ab->board[ab->x[i] + dx][ab->y[i] + dy] != NO_BALL)
						return 0;
				}
				ab->broadSizeMove = 1;
				return 1;
			}
		}
	}

	return allow;
}

void isRightCliked(AbaloneBoard *ab,int x, int y)
{

	if (canMove(ab,x,y))
	{
		if (ab->broadSizeMove == 0)
			move(ab, ab->x[ab->selectedBalls - 1], ab->y[ab->selectedBalls - 1], x, y);
		else
		{
			int i;
			for (i = 0; i < ab->selectedBalls; i++)
			{
				ab->board[ab->x[i]][ab->y[i]] = NO_BALL;
				ab->board[ab->x[i] + ab->dxj][ab->y[i] + ab->dyj] = ab->turn;
			}
		}
		ab->turn = (ab->turn == WHITE) ? BLACK : WHITE;
		ab->selectedBalls = 0;
	}
}

void isLeftCliked(AbaloneBoard *ab, int x, int y)
{
	int selectedColor,color;
	if (ab->turn == BLACK)
	{
		selectedColor = SELECTED_BLACK;
		color = BLACK;
	}
	else  if (ab->turn == WHITE)
	{
		selectedColor = SELECTED_WHITE;
		color = WHITE;
	}

	if (ab->board[x][y] == selectedColor)
	{
		int i;
		for (i = 0; i < ab->selectedBalls; i++)
			if (ab->x[i] == x && ab->y[i] == y)
				break;

		ab->selectedBalls--;

		if (i < ab->selectedBalls)
		{
			ab->x[i] = ab->x[ab->selectedBalls];
			ab->y[i] = ab->y[ab->selectedBalls];
		}

		ab->board[x][y] = color;
#ifdef _DEBUG
		if(color==BLACK)
			printf("%c%d = BLACK\n",65+x,1+y);
		else
			printf("%c%d = WHITE\n", 65+x, 1+y);
#endif
	}
	else if (ab->selectedBalls < 3 && ab->board[x][y] == color)
	{
		int allow = (ab->selectedBalls == 0) ? 1 : 0;

		if (ab->selectedBalls != 0)
		{
			if (abs(ab->x[0] - x) > abs(ab->x[ab->selectedBalls - 1] - x))
			{
				int tmp = ab->x[0];
				ab->x[0] = ab->x[ab->selectedBalls - 1];
				ab->x[ab->selectedBalls - 1] = tmp;
			}
			if (abs(ab->y[0] - y) > abs(ab->y[ab->selectedBalls - 1] - y))
			{
				int tmp = ab->y[0];
				ab->y[0] = ab->y[ab->selectedBalls - 1];
				ab->y[ab->selectedBalls - 1] = tmp;
			}
		}
		if (ab->selectedBalls == 1)
		{
			if (abs(ab->x[0] - x) == 1 && abs(y - ab->y[0]) == 1)
				allow = ab->x[0] - x == ab->y[0] - y;
			else
				allow = (abs(ab->x[0] - x) <= 1 && abs(y - ab->y[0]) <= 1);
		}
		else if (ab->selectedBalls == 2)
		{
			if (ab->x[0] != ab->x[1] && ab->y[0] != ab->y[1])
				allow = (abs(ab->x[0] - x) == 1 && abs(y - ab->y[0]) == 1);
			else if (ab->x[0] != ab->x[1])
				allow = (abs(ab->x[0] - x) == 1 && abs(y - ab->y[0]) == 0);
			else
				allow = (abs(ab->x[0] - x) == 0 && abs(y - ab->y[0]) == 1);
		}

		if (allow)
		{
			ab->board[x][y] = selectedColor;
			ab->x[ab->selectedBalls] = x;
			ab->y[ab->selectedBalls] = y;
			ab->selectedBalls++;
#ifdef _DEBUG
			if(selectedColor==SELECTED_BLACK)
				printf("%c%d = SELECTED_BLACK\n", 65+x, 1+y);
			else
				printf("%c%d = SELECTED_WHITE\n", 65+x, 1+y);
#endif
		}
	}
}
