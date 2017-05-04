#ifndef  ABALONE_BOARD_H
#define  ABALONE_BOARD_H

#include "TextureManager.h"
#include "EventManager.h"

#define BLACK -1
#define SELECTED_BLACK 2*BLACK
#define NO_BALL 0
#define WHITE -BLACK
#define SELECTED_WHITE 2*WHITE
#define SIZE 9
#define BALL_TEXTURE_SIZE 54

typedef enum Direction
{
	NORTH_EAST, NORTH_WEST, EAST, WEST, SOUTH_EAST, SOUTH_WEST
}Direction;

typedef struct AbaloneBoard
{
	int board[SIZE][SIZE];
	Drawable *black,*white, boardDrawable;
	int blackBallsCount, whiteBallsCount;
	int turn,selectedBalls;
	int x[3], y[3];
}AbaloneBoard;

AbaloneBoard* newAbaloneBoard(EventManager *em, SDL_Renderer *ren, TextureManager *tm, int blackBallCount, int whiteBallCount);
void setDefaultConf(AbaloneBoard *ab);
void drawBoard(AbaloneBoard *ab, EventManager *em);
void setConf(AbaloneBoard * ab,int board[9][9]);
void deleteAdaloneBoard(AbaloneBoard *ab);
void convertCoord(int boardLetter, int boardNumber, int *screenX, int *screenY);
void setDrawableCoord(AbaloneBoard *ab);
int canMoveDir(Direction dir, AbaloneBoard *ab);

#endif // ! ABALONE_BOARD_H

