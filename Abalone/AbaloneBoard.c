#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "AbaloneBoard.h"
#include "minIA.h"

AbaloneBoard *getAbaloneBoard(AbaloneBoard *ab);
int handleClik(Event *e);

AbaloneBoard * newAbaloneBoard(EventManager *em, SDL_Renderer *ren, TextureManager *tm, int blackBallCount, int whiteBallCount)
{
	AbaloneBoard *ab = newTheoricalAbaloneBoard(blackBallCount, whiteBallCount);
	int i = 0, j = 0;

	ab->black = malloc(sizeof(Drawable)*blackBallCount);
	ab->white = malloc(sizeof(Drawable)*whiteBallCount);

	ab->boardDrawable = getDrawable(ren, "./Image/AbaloneBoard.png", 0, 0);
	for (i = 0;i < blackBallCount;i++)
		ab->black[i] = getDrawable(ren, "./Image/blackBall.png", 0, 0);
	
	for (i = 0;i < whiteBallCount;i++)
		ab->white[i] = getDrawable(ren, "./Image/whiteBall.png", 0, 0);

	getDrawable(ren, "./Image/selectedBall.png", 0, 0);

	getAbaloneBoard(ab);

	addCallback(handleClik, SDL_EVENT);

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

void setDefaultConf(AbaloneBoard * ab)
{
	int i;

	for (i = 0;i < 5;i++)
		ab->board[0][i] = BLACK;
	for (i = 0;i < 6;i++)
		ab->board[1][i] = BLACK;
	for (i = 2;i < 5;i++)
		ab->board[2][i] = BLACK;
	for (i = 0;i < 2;i++)
		ab->board[2][i] = NO_BALL;
	for (i = 5;i < 7;i++)
		ab->board[2][i] = NO_BALL;
	for (i = 0;i < 8;i++)
		ab->board[3][i] = NO_BALL;
	for (i = 0;i < 9;i++)
		ab->board[4][i] = NO_BALL;
	for (i = 1;i < 9;i++)
		ab->board[5][i] = NO_BALL;
	for (i = 2;i < 4;i++)
		ab->board[6][i] = NO_BALL;
	for (i = 7;i < 9;i++)
		ab->board[6][i] = NO_BALL;

	for (i = 4;i < 5+4;i++)
		ab->board[8][i] = WHITE;
	for (i = 3;i < 6+3;i++)
		ab->board[7][i] = WHITE;
	for (i = 2+2;i < 5+2;i++)
		ab->board[6][i] = WHITE;
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

}

void setConf(AbaloneBoard * ab,int board[9][9])
{
	int i = 0, j = 0;
	for (i = 0;i < SIZE;i++)
		for (j = 0;j < SIZE;j++)
			ab->board[i][j]=board[i][j];
}

void deleteAdaloneBoard(AbaloneBoard * ab)
{
	free(ab->black);
	free(ab->white);
	free(ab);
}

void convertCoord(int boardLetter, int boardNumber, int * screenX, int * screenY)
{
	/*boardLetter *= 66;
	boardNumber *= 66;
	boardNumber *= cos(30. / 180.*M_PI);
	boardLetter += 238;
	boardNumber += 63;
	boardNumber = WINDOW_HEIGHT - boardNumber;
	*screenX = boardLetter;
	*screenY = boardNumber;*/

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
/*	screenY = WINDOW_HEIGHT - screenY;
	screenY -= 63;
	screenX -= 238;
	screenY /= cos(30. / 180.*M_PI);
	*boardL = screenX / 66;
	*boardN = screenY / 66;*/
	*boardL = (int)(floor((screenY - 293.) / 57.)); ////Old version
	*boardN = (int)((screenX - (109 + abs(*boardL)*cos(60. / 180.*M_PI) * 66)) / 66);
	*boardL *= -1;
	*boardL += 4;
	if (*boardL > 4)
		*boardN += *boardL - 4;
}

void setDrawableCoord(AbaloneBoard * ab)
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
					ab->black[bc].tex = getTextureFromRoot("./Image/selectedBall.png");
				else
					ab->black[bc].tex = getTextureFromRoot("./Image/blackBall.png");

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
					ab->white[wc].tex = getTextureFromRoot("./Image/selectedBall.png");
				else
					ab->white[wc].tex = getTextureFromRoot("./Image/whiteBall.png");
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

void northEaster(int x1, int y1, int x2, int y2, int *fx, int *fy)
{
	if (x1 > x2)
	{
		*fx = x1;
		*fy = y1;
	}
	else
	{
		*fx = x2;
		*fy = y2;
	}
}
void northWester(int x1, int y1, int x2, int y2, int *fx, int *fy)
{

}
void easter(int x1, int y1, int x2, int y2, int *fx, int *fy)
{

}
void wester(int x1, int y1, int x2, int y2, int *fx, int *fy)
{

}
void southEaster(int x1, int y1, int x2, int y2, int *fx, int *fy)
{

}
void southWester(int x1, int y1, int x2, int y2, int *fx, int *fy)
{

}

/*Penser a mettre à jour dans le board ab les variables
**	- turn
**	- x[], y[]
**  - selectedBalls
*/
int canMoveDir(Direction dir, AbaloneBoard *ab)
{
	int x = 0, y = 0, allow = 0, i = 0;

	if (ab->selectedBalls == 0)
		return 0;

	switch (dir)
	{
	case NORTH_EAST:
		if (isCorrectForXYAlign(ab->x, ab->y, ab->selectedBalls))
		{
			for (i = 0;i < ab->selectedBalls;i++)
				northEaster(x, y, ab->x[i], ab->y[i], &x, &y);

			return canMove(ab,x + 1, y + 1);
		}
		break;
	case NORTH_WEST:
		if (isCorrectForXYAlign(ab->x, ab->y, ab->selectedBalls))
		{
			for (i = 0;i < ab->selectedBalls;i++)
				northWester(x, y, ab->x[i], ab->y[i], &x, &y);

			return canMove(ab, x + 1, y - 1);
		}
		break;
	case EAST:
		if (isCorrectForXAlign(ab->x, ab->y, ab->selectedBalls))
		{
			for (i = 0;i < ab->selectedBalls;i++)
				easter(x, y, ab->x[i], ab->y[i], &x, &y);

			return canMove(ab, x, y + 1);
		}
		break;
	case WEST:
		if (isCorrectForXAlign(ab->x, ab->y, ab->selectedBalls))
		{
			for (i = 0;i < ab->selectedBalls;i++)
				wester(x, y, ab->x[i], ab->y[i], &x, &y);

			return canMove(ab, x, y - 1);
		}
		break;
	case SOUTH_EAST:
		if (isCorrectForXYAlign(ab->x, ab->y, ab->selectedBalls))
		{
			for (i = 0;i < ab->selectedBalls;i++)
				southEaster(x, y, ab->x[i], ab->y[i], &x, &y);

			return canMove(ab, x - 1, y + 1);
		}
		break;
	case SOUTH_WEST:
		if (isCorrectForXYAlign(ab->x, ab->y, ab->selectedBalls))
		{
			for (i = 0;i < ab->selectedBalls;i++)
				southWester(x, y, ab->x[i], ab->y[i], &x, &y);

			return canMove(ab, x - 1, y - 1);
		}
		break;
	default:
		break;
	}
	return 0;
}

int handleClik(Event * e)
{
	int i = 0, j = 0, x = 0, y = 0, wc = 0, bc = 0;
	SDL_Event *evt = (SDL_Event*)(e->data);
	AbaloneBoard *ab = getAbaloneBoard(NULL);

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

int getOpponentBallsCount(int ox, int oy, int dx, int dy, int color)
{
	int c = 0;
	AbaloneBoard *ab = getAbaloneBoard(NULL);

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
	int selectedColor, color, opColor;

	if (ab->turn == BLACK)
	{
		selectedColor = SELECTED_BLACK;
		color = BLACK;
		opColor = WHITE;
	}
	else  if (ab->turn == WHITE)
	{
		selectedColor = SELECTED_WHITE;
		color = WHITE;
		opColor = BLACK;
	}
	for (i = 0; i < ab->selectedBalls; i++)
		ab->board[ab->x[i]][ab->y[i]] = color;

	ab->board[ox][oy] = NO_BALL;
	
	if (ab->jumpOver > 0)
	{
		if (destx + ab->dxj >= SIZE || desty + ab->dyj >= SIZE)
		{
			if (-color == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else if (ab->board[destx + ab->dxj][desty + ab->dyj] == OUT_ZONE)
		{
			if (-color == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else if(ab->jumpOver == 1)
			ab->board[destx + ab->dxj][desty + ab->dyj] = -color;
		else if (destx + 2 * ab->dxj >= SIZE || desty + 2 * ab->dyj >= SIZE)
		{
			if (-color == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else if (ab->board[destx + 2*ab->dxj][desty + 2*ab->dyj] == OUT_ZONE)
		{
			if (-color == BLACK)
				ab->blackDeadBalls++;
			else
				ab->whiteDeadBalls++;
		}
		else
			ab->board[destx + 2*ab->dxj][desty + 2*ab->dyj] = -color;
	}

	ab->board[destx][desty] = color;
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
	else
	{
		*fx = x2;
		*fy = y2;
		*rx = x1;
		*ry = y1;
	}
}

int canMove(AbaloneBoard *ab,int x, int y)
{
	int allow = 0;
	ab->jumpOver = 0;

	if (ab->selectedBalls != 0)
	{
		int selectedColor, color;
		int i = 0, j = 0;

		for (j = 0;j < ab->selectedBalls - 1;j++)
			for (i = 0;i < ab->selectedBalls - 1;i++)
				nearest(ab->x[i], ab->y[i], ab->x[i + 1], ab->y[i + 1], x, y, &ab->x[i], &ab->y[i], &ab->x[i + 1], &ab->y[i + 1]);

		int dx = x - ab->x[0], dy = y - ab->y[0];

		if (abs(dx) > 1 || abs(dy > 1))
			return 0;

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

		if (color + ab->board[x][y] == 0)
		{
			if (ab->selectedBalls == 1)
				return 0;
			double a;
			double b;
			if (ab->x[0] != ab->x[1])
			{
				a = (ab->y[0] - ab->y[1]) / (ab->x[0] - ab->x[1]);
				b = ab->y[0] - a*ab->x[0];
				if (y != a*x + b)
					return 0;
			}
			else
			{
				a = (ab->x[0] - ab->x[1])/(ab->y[0] - ab->y[1]);
				b = ab->x[0] - a*ab->y[0];
				if (x != a*y + b)
					return 0;
			}

			ab->jumpOver++;
			ab->dxj = dx;
			ab->dyj = dy;


			if (ab->x[0] + 2*dx >= SIZE || ab->y[0] + 2*dy >= SIZE)
				return 1;

			if (ab->board[ab->x[0] + 2*dx][ab->y[0] + 2*dy] == NO_BALL || ab->board[ab->x[0] + 2 * dx][ab->y[0] + 2 * dy] == OUT_ZONE)
				return 1;
			
			if (ab->board[ab->x[0] + 2*dx][ab->y[0] + 2*dy] == -color && ab->selectedBalls == 3)
			{
				ab->jumpOver++;

				if (ab->x[0] + 3*dx >= SIZE || ab->y[0] + 3*dy >= SIZE)
					return 1;

				return (ab->board[ab->x[0] + 3 * dx][ab->y[0] + 3 * dy] == NO_BALL) || (ab->board[ab->x[0] + 3 * dx][ab->y[0] + 3 * dy] == OUT_ZONE);
			}

			ab->jumpOver = 0;
			return 0;
		}
		else if (ab->board[x][y] == 0)
		{
			if (ab->selectedBalls == 1)
				allow = (abs(ab->x[0] - x) <= 1 && abs(y - ab->y[0]) <= 1) && (x != ab->x[0] || y != ab->y[0]);
			else //if (ab->selectedBalls == 2)
			{
				if (ab->x[0] != ab->x[1] && ab->y[0] != ab->y[1])
					allow = (abs(ab->x[0] - x) == 1 && abs(y - ab->y[0]) == 1);
				else if (ab->x[0] != ab->x[1])
					allow = (abs(ab->x[0] - x) == 1 && abs(y - ab->y[0]) == 0);
				else
					allow = (abs(ab->x[0] - x) == 0 && abs(y - ab->y[0]) == 1);
			}
		}
	}

	return allow;
}

void isRightCliked(AbaloneBoard *ab,int x, int y)
{

	if (canMove(ab,x,y))
	{
		move(ab, ab->x[ab->selectedBalls-1], ab->y[ab->selectedBalls-1], x, y);
		ab->turn = (ab->turn == WHITE) ? BLACK : WHITE;
		ab->selectedBalls = 0;

		if (ab->turn == WHITE) {
			min_ia_start(ab);
		}
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

AbaloneBoard * getAbaloneBoard(AbaloneBoard * ab)
{
	static AbaloneBoard *m_ab = NULL;
	if (m_ab == NULL)
		m_ab = ab;
	return m_ab;
}
