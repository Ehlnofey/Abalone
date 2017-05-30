#ifndef SELECT_BOARD_INTERFACE_H
#define SELECT_BOARD_INTERFACE_H

#include "EventManager.h"
#include "TextureManager.h"

typedef enum BOARD_TYPE { NO_BOARD_TYPE, DEFAULT_BOARD, BELGIAN_DAISY_BOARD } BOARD_TYPE;

typedef struct SelectBoardInterface
{
	Drawable DefaultBoard, BerlgianDaisyBoard;
	BOARD_TYPE boardType;
}SelectBoardInterface;

SelectBoardInterface* newSelectBoardInterface(TextureManager *tm, SDL_Renderer *ren, EventManager *em);
void drawSelectBoardInterface(SelectBoardInterface* spli, EventManager *em);
void deleteSelectBoardInterface(SelectBoardInterface *spi);
#endif
