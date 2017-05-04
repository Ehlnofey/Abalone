#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "AbaloneBoard.h"

AbaloneBoard *getAbaloneBoard(AbaloneBoard *ab);
int handleClik(Event *e);
void isLeftCliked(int x, int y);
void isRightCliked(int x, int y);

AbaloneBoard * newAbaloneBoard(EventManager *em, SDL_Renderer *ren, TextureManager *tm, int blackBallCount, int whiteBallCount)
{
	AbaloneBoard *ab = malloc(sizeof(AbaloneBoard));
	int i = 0, j = 0;

	ab->black = malloc(sizeof(Drawable)*blackBallCount);
	ab->white = malloc(sizeof(Drawable)*whiteBallCount);
	ab->blackBallsCount = blackBallCount;
	ab->whiteBallsCount = whiteBallCount;
	ab->turn = BLACK;
	ab->selectedBalls = 0;

	for (i = 0;i < SIZE;i++)
		for (j = 0;j < SIZE;j++)
			ab->board[i][j] = NO_BALL;

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

void setDefaultConf(AbaloneBoard * ab)
{
	int i = 0;
	for (i = 0;i < 5;i++)
		ab->board[0][i] = BLACK;
	for (i = 0;i < 6;i++)
		ab->board[1][i] = BLACK;
	for (i = 2;i < 5;i++)
		ab->board[2][i] = BLACK;

	for (i = 0;i < 5;i++)
		ab->board[8][i] = WHITE;
	for (i = 0;i < 6;i++)
		ab->board[7][i] = WHITE;
	for (i = 2;i < 5;i++)
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
/*Penser a mettre à jour dans le board ab les variables
**	- turn
**	- x[], y[]
**  - selectedBalls
*/
int canMoveDir(Direction dir, AbaloneBoard *ab)
{
	int x, y, allow = 0;

	if (ab->selectedBalls == 0)
		return 0;

	switch (dir)
	{
	case NORTH_EAST:
		if (ab->selectedBalls == 1 && ab->x[0] >= 0 && ab->x[0] < SIZE - 1 && ab->y[0] >= 0 && ab->y[0] < SIZE - 1)
			return ab->board[ab->x[0]+1][ab->y[0]+1] == NO_BALL;
		else if (ab->selectedBalls == 2 && abs(ab->x[0] - ab->x[1]) == 1 && abs(ab->y[0] - ab->y[1]) == 1)
		{
			x = max(ab->x[0], ab->x[1]);
			y = max(ab->y[0], ab->y[1]);

			if (x + 1 >= 0 && x + 1 < SIZE && y + 1 >= 0 && y + 1 < SIZE)
			{
				allow = ab->board[x + 1][y + 1] == NO_BALL;

				if (x + 2 >= 0 && x + 2 < SIZE && y + 2 >= 0 && y + 2 < SIZE)
					allow = allow || ((ab->board[x + 1][y + 1] == -ab->turn) && (ab->board[x + 2][y + 2] == NO_BALL));
			}

			return allow;
		}
		else
		{

		}
		break;
	case NORTH_WEST:
		if (ab->selectedBalls == 1 && ab->x[0] >= 0 && ab->x[0] < SIZE - 1 && ab->y[0] > 0 && ab->y[0] < SIZE)
			return ab->board[ab->x[0] + 1][ab->y[0] - 1] == NO_BALL;
		break;
	case EAST:
		if (ab->selectedBalls == 1 && ab->x[0] >= 0 && ab->x[0] < SIZE && ab->y[0] >= 0 && ab->y[0] < SIZE - 1)
			return ab->board[ab->x[0]][ab->y[0] + 1] == NO_BALL;
		break;
	case WEST:
		if (ab->selectedBalls == 1 && ab->x[0] >= 0 && ab->x[0] < SIZE && ab->y[0] > 0 && ab->y[0] < SIZE)
			return ab->board[ab->x[0]][ab->y[0] - 1] == NO_BALL;
		break;
	case SOUTH_EAST:
		if (ab->selectedBalls == 1 && ab->x[0] > 0 && ab->x[0] < SIZE && ab->y[0] >= 0 && ab->y[0] < SIZE - 1)
			return ab->board[ab->x[0] - 1][ab->y[0] + 1] == NO_BALL;
		break;
	case SOUTH_WEST:
		if (ab->selectedBalls == 1 && ab->x[0] > 0 && ab->x[0] < SIZE && ab->y[0] > 0 && ab->y[0] < SIZE)
			return ab->board[ab->x[0] - 1][ab->y[0] - 1] == NO_BALL;
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
			isLeftCliked(x,y);
	}
	else if (evt->type == SDL_MOUSEBUTTONUP && evt->button.button == SDL_BUTTON_RIGHT)
	{
		screenToBoard(evt->button.x, evt->button.y, &x, &y);

		if (x >= 0 && x < SIZE && y >= 0 && y < SIZE)
			isRightCliked(x, y);
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
		for (j = 0; j < ab->selectedBalls; j++)
			ab->board[ab->x[i]][ab->y[j]] = color;

	ab->board[ox][oy] = NO_BALL;
	ab->board[destx][desty] = color;
}

int canMove(int x, int y)
{
	AbaloneBoard *ab = getAbaloneBoard(NULL);
	int allow = 0;

	if (ab->selectedBalls != 0)
	{
		int selectedColor, color;

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
			//Clique sur l'autre couleur
		}
		else if (ab->board[x][y] == 0)
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

void isRightCliked(int x, int y)
{
	AbaloneBoard *ab = getAbaloneBoard(NULL);

	if (canMove(x,y))
	{
		move(ab, ab->x[ab->selectedBalls-1], ab->y[ab->selectedBalls-1], x, y);
		ab->turn = (ab->turn == WHITE) ? BLACK : WHITE;
		ab->selectedBalls = 0;
	}
}

void isLeftCliked(int x, int y)
{
	AbaloneBoard *ab = getAbaloneBoard(NULL);

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
	}
	else if (ab->selectedBalls < 3)
	{
		int allow = (ab->selectedBalls == 0 && ab->board[x][y] == color) ? 1 : 0;

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
		if(ab->selectedBalls==1)
			allow = (abs(ab->x[0] - x) <= 1 && abs(y - ab->y[0]) <= 1) || (abs(ab->x[0] - x) <= 1 && abs(y - ab->y[0]) <= 1);
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
