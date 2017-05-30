#include "SelectPlayerInterface.h"
#include "Button.h"

int handleClick(void *handler, Event *e)
{
	SelectPlayerInterface *spi = (SelectPlayerInterface*)handler;
	SDL_Event *evt = (SDL_Event*)(e->data);

	if (evt->type == SDL_MOUSEBUTTONUP&&evt->button.button == SDL_BUTTON_LEFT)
	{
		if (drawableIsCliked(&spi->IAvsIA, evt))
			spi->gameMode = IA_VS_IA;
		else if (drawableIsCliked(&spi->PlayervsIA, evt))
			spi->gameMode = PLAYER_VS_IA;
		else if (drawableIsCliked(&spi->PlayervsPlayer, evt))
			spi->gameMode = PLAYER_VS_PLAYER;
	}

	return 0;
}

SelectPlayerInterface * newSelectPlayerInterface(TextureManager *tm, SDL_Renderer *ren, EventManager *em)
{
	SelectPlayerInterface *spi = malloc(sizeof(SelectPlayerInterface));

	spi->IAvsIA = getDrawable(tm, ren, "./Image/IAvsIA.png",50,275);

	spi->PlayervsIA = getDrawable(tm, ren, "./Image/PlayervsIA.png",300,275);

	spi->PlayervsPlayer = getDrawable(tm, ren, "./Image/PlayervsPlayer.png",550,275);

	spi->Band = getDrawable(tm, ren, "./Image/Band.png", -20, -20);

	spi->gameMode = NO_GAME_MODE;

	addCallback(em, handleClick, SDL_EVENT, spi);

	return spi;
}

void drawSelectPlayerInterface(SelectPlayerInterface * spi, EventManager * em)
{
	if (spi->gameMode==NO_GAME_MODE)
	{
		Event e;
		e.code = DRAW;
		e.data = &spi->IAvsIA;
		
		e.haveToBeDelete = 0;
		pushEvent(e);

		e.data = &spi->PlayervsIA;
		pushEvent(e);
		
		e.data = &spi->PlayervsPlayer;
		pushEvent(e);

		//e.data = &spi->Band;
		//pushEvent(e);
	}
}

void deleteSelectPlayerInterface(SelectPlayerInterface * spi)
{
	free(spi);
}
