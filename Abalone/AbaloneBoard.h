#ifndef  ABALONE_BOARD_H
#define  ABALONE_BOARD_H

#include "TextureManager.h"
#include "EventManager.h"

#define BLACK -1
#define NO_BALL 0
#define WHITE 1
#define SIZE 9
#define BALL_TEXTURE_SIZE 58

typedef struct AbaloneBoard
{
	int board[SIZE][SIZE];
	Drawable *black,*white, boardDrawable;
	int *blackIsSelected, *whiteIsSelected;
	int blackBallCount, witheBallCount;
}AbaloneBoard;

AbaloneBoard* newAbaloneBoard(EventManager *em, SDL_Renderer *ren, TextureManager *tm, int blackBallCount, int whiteBallCount);
void setDefaultConf(AbaloneBoard *ab);
void drawBoard(AbaloneBoard *ab, EventManager *em);
void setConf(AbaloneBoard * ab,int board[9][9]);
void deleteAdaloneBoard(AbaloneBoard *ab);
void convertCoord(int boardLetter, int boardNumber, int *screenX, int *screenY);
void setDrawableCoord(AbaloneBoard *ab);

#endif // ! ABALONE_BOARD_H

