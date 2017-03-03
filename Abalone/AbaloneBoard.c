#include "AbaloneBoard.h"

AbaloneBoard *getAbaloneBoard(AbaloneBoard *ab);
int handleClik(Event *e);
void isLeftCliked(int x, int y);

AbaloneBoard * newAbaloneBoard(EventManager *em, SDL_Renderer *ren, TextureManager *tm, int blackBallCount, int whiteBallCount)
{
	AbaloneBoard *ab = malloc(sizeof(AbaloneBoard));
	int i = 0, j = 0;

	ab->black = malloc(sizeof(Drawable)*blackBallCount);
	ab->white = malloc(sizeof(Drawable)*whiteBallCount);

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
	*boardL = floor((screenY - 293.) / 57.);
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
				convertCoord(i, j, &ab->black[bc].dst.x, &ab->black[bc].dst.y);
				if (ab->board[i][j] == SELECTED_BLACK)
					ab->black[bc].tex = getTextureFromRoot("./Image/selectedBall.png");
				else
					ab->black[bc].tex = getTextureFromRoot("./Image/blackBall.png");

				bc++;
			}
			else if (ab->board[i][j] == WHITE || ab->board[i][j] == SELECTED_WHITE)
			{
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
	else if (evt->type == SDL_MOUSEBUTTONUP&&evt->button.button ==SDL_BUTTON_RIGHT)
	{

	}

	return 0;
}

void isLeftCliked(int x, int y)
{
	AbaloneBoard *ab = getAbaloneBoard(NULL);

	if (ab->board[x][y] == BLACK)
		ab->board[x][y] = SELECTED_BLACK;
	else if (ab->board[x][y] == SELECTED_BLACK)
		ab->board[x][y] = BLACK;
	else if (ab->board[x][y] == WHITE)
		ab->board[x][y] = SELECTED_WHITE;
	else if (ab->board[x][y] == SELECTED_WHITE)
		ab->board[x][y] = WHITE;
}

AbaloneBoard * getAbaloneBoard(AbaloneBoard * ab)
{
	static AbaloneBoard *m_ab = NULL;
	if (m_ab == NULL)
		m_ab = ab;
	return m_ab;
}
