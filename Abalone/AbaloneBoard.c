#include "AbaloneBoard.h"

AbaloneBoard *getAbaloneBoard(AbaloneBoard *ab);
int handleClik(Event *e);
void isLeftCliked(int pos, int color);

AbaloneBoard * newAbaloneBoard(EventManager *em, SDL_Renderer *ren, TextureManager *tm, int blackBallCount, int whiteBallCount)
{
	AbaloneBoard *ab = malloc(sizeof(AbaloneBoard));
	int i = 0, j = 0;

	ab->black = malloc(sizeof(Drawable)*blackBallCount);
	ab->white = malloc(sizeof(Drawable)*whiteBallCount);
	ab->blackIsSelected = malloc(sizeof(int)*blackBallCount);
	ab->whiteIsSelected = malloc(sizeof(int)*whiteBallCount);
	ab->blackBallCount = blackBallCount;
	ab->witheBallCount = whiteBallCount;

	for (i = 0;i < SIZE;i++)
		for (j = 0;j < SIZE;j++)
			ab->board[i][j] = NO_BALL;

	ab->boardDrawable = getDrawable(ren, "./Image/AbaloneBoard.png", 0, 0);
	for (i = 0;i < blackBallCount;i++)
	{
		ab->black[i] = getDrawable(ren, "./Image/blackBall.png", 0, 0);
		ab->blackIsSelected[i] = 0;
	}
	
	for (i = 0;i < whiteBallCount;i++)
	{
		ab->white[i] = getDrawable(ren, "./Image/whiteBall.png", 0, 0);
		ab->whiteIsSelected[i] = 0;
	}

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
			if (ab->board[i][j] == BLACK)
			{
				e.data = &ab->black[bc];
				bc++;
				pushEvent(e);
			}
			else if (ab->board[i][j] == WHITE)
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
	*boardL = (screenY - 293) / 57;
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
			if (ab->board[i][j] == BLACK)
			{
				convertCoord(i, j, &ab->black[bc].dst.x, &ab->black[bc].dst.y);
				bc++;
			}
			else if (ab->board[i][j] == WHITE)
			{
				convertCoord(i, j, &ab->white[wc].dst.x, &ab->white[wc].dst.y);
				wc++;
			}
		}
	}
}

int handleClik(Event * e)
{
	int i = 0, j = 0, x = 0, y = 0, wc = 0, bc = 0;
	SDL_Event *evt = (SDL_Event*)(e->data);
	AbaloneBoard *ab = getAbaloneBoard(NULL);

	if (evt->type == SDL_MOUSEBUTTONUP&&evt->button.clicks==SDL_BUTTON_LEFT)
	{
		screenToBoard(evt->button.x, evt->button.y, &x, &y);

		if (x >= 0 && x < SIZE && y >= 0 && y < SIZE)
		{
			for (i = 0;i < x;i++)
			{
				for (j = 0;j < y;j++)
				{
					if (ab->board[i][j] == BLACK)
						bc++;
					else
						wc++;
				}
			}

			isLeftCliked((ab->board[x][y] == BLACK) ? bc : wc, (ab->board[x][y] == BLACK) ? BLACK : WHITE);
		}
	}
	else if (evt->type == SDL_MOUSEBUTTONUP&&evt->button.clicks==SDL_BUTTON_RIGHT)
	{

	}

	return 0;
}

void isLeftCliked(int pos, int color)
{
	AbaloneBoard *ab = getAbaloneBoard(NULL);
	int *isSelected;
	Drawable *d;

	if (color == BLACK)
	{
		isSelected = ab->blackIsSelected;
		d = ab->black;
	}
	else
	{
		isSelected = ab->whiteIsSelected;
		d = ab->white;
	}

	isSelected[pos] = (isSelected[pos] + 1) % 2;
	if (isSelected[pos] == 0)
		d[pos].tex = getTextureFromRoot("./Image/selectedBall.png");
	else
		d[pos].tex = getTextureFromRoot("./Image/selectedBall.png");
}

AbaloneBoard * getAbaloneBoard(AbaloneBoard * ab)
{
	static AbaloneBoard *m_ab = NULL;
	if (m_ab == NULL)
		m_ab = ab;
	return m_ab;
}
