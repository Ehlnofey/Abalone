#ifndef SELECT_PLAYER_INTERFACE_H
#define SELECT_PLAYER_INTERFACE_H

#include "EventManager.h"
#include "TextureManager.h"

typedef struct SelectPlayerInterface
{
	Drawable *IAvsIA, *PlayervsIA, *PlayervsPlayer;
}SelectPlayerInterface;

SelectPlayerInterface* newSelectPlayerInterface();
void drawSelectPlayerInterface(SelectPlayerInterface* spli,EventManager *em);

#endif