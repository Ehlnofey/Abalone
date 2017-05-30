#include "SelectBoardInterface.h"
#include "Button.h"

int handleClick_SelectBoardInterface(void *handler, Event *e)
{
	SelectBoardInterface *sbi = (SelectBoardInterface*)handler;
	SDL_Event *evt = (SDL_Event*)(e->data);

	if (sbi->boardType != NO_BOARD_TYPE)
		return 0;

	if (evt->type == SDL_MOUSEBUTTONUP&&evt->button.button == SDL_BUTTON_LEFT)
	{
		if (drawableIsCliked(&sbi->DefaultBoard, evt))
			sbi->boardType = DEFAULT_BOARD;
		else if (drawableIsCliked(&sbi->BerlgianDaisyBoard, evt))
			sbi->boardType = BELGIAN_DAISY_BOARD;
	}

	return 0;
}

SelectBoardInterface * newSelectBoardInterface(TextureManager *tm, SDL_Renderer *ren, EventManager *em)
{
	SelectBoardInterface *sbi = malloc(sizeof(SelectBoardInterface));

	sbi->DefaultBoard = getDrawable(tm, ren, "./Image/defaut.png", 33, 150);

	sbi->BerlgianDaisyBoard = getDrawable(tm, ren, "./Image/belgianDaisy.png", 416, 150);

	sbi->boardType = NO_BOARD_TYPE;

	addCallback(em, handleClick_SelectBoardInterface, SDL_EVENT, sbi);

	return sbi;
}

void drawSelectBoardInterface(SelectBoardInterface * sbi, EventManager * em)
{
	if (sbi->boardType == NO_BOARD_TYPE)
	{
		Event e;
		e.code = DRAW;
		e.data = &sbi->DefaultBoard;

		e.haveToBeDelete = 0;
		pushEvent(e);

		e.data = &sbi->BerlgianDaisyBoard;
		pushEvent(e);
	}
}

void deleteSelectBoardInterface(SelectBoardInterface * sbi)
{
	free(sbi);
}
