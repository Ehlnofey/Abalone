#ifndef SELECT_PLAYER_INTERFACE_H
#define SELECT_PLAYER_INTERFACE_H

#include "EventManager.h"
#include "TextureManager.h"

typedef enum GameMode {NO_GAME_MODE, IA_VS_IA, PLAYER_VS_IA, PLAYER_VS_PLAYER} GameMode;

typedef struct SelectPlayerInterface
{
	Drawable IAvsIA, PlayervsIA, PlayervsPlayer, Band;
	GameMode gameMode;
}SelectPlayerInterface;

SelectPlayerInterface* newSelectPlayerInterface(TextureManager *tm, SDL_Renderer *ren, EventManager *em);
void drawSelectPlayerInterface(SelectPlayerInterface* spli,EventManager *em);
void deleteSelectPlayerInterface(SelectPlayerInterface *spi);

#endif