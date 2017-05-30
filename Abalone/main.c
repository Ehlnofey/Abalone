#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "EventManager.h"
#include "WindowManager.h"
#include "AbaloneBoard.h"
#include "minimax.h"
#include "SelectPlayerInterface.h"


int handle_EVENT(void* handler, Event *e)
{
	int *iaPlay = (int*)(handler);
	SDL_Event *evt = (SDL_Event*)(e->data);

	if (evt->type == SDL_KEYDOWN)
		if (evt->key.keysym.sym == (SDL_Keycode)SDLK_p)
			*iaPlay = 0;

	return 0;
}

void startGame(AbaloneBoard **ab, EventManager *em, SDL_Renderer *ren, TextureManager *tm, int (*board)[9][9])
{
	*ab = newAbaloneBoard(em, ren, tm, 14, 14);
	setConf(*ab, board);
}

void playGame(AbaloneBoard *ab, EventManager *em, TextureManager *tm,GameMode gm, clock_t *start, int *iaPlay, int *cnt, EvalWeights* blackEW, EvalWeights* whiteEW)
{
	setDrawableCoord(ab, tm);
	drawBoard(ab, em);
	if (clock() - *start > 1 && *iaPlay)
	{
		if (ab->turn == BLACK && gm == IA_VS_IA)
			start_ia(ab, blackEW, 3, 0);
		else if (ab->turn == WHITE && (gm == IA_VS_IA || gm == PLAYER_VS_IA))
			start_ia(ab, whiteEW, 3, 0);
		*start = clock();
	}
	switch (someoneWin(ab))
	{
	case BLACK:
		*cnt = 0;
		printf("Noir a gagne !\n");
		break;
	case WHITE:
		*cnt = 0;
		printf("Blanc a gagne !\n");
		break;
	}
}

int main(int argc, char * argv[])
{
	EventManager myEM;
	BuildedWindow *myWindow;
	TextureManager *tm;
	AbaloneBoard *ab = NULL;
	SelectPlayerInterface *spi;
	EvalWeights evalWeightsD, evalWeightsA;
	int cnt = 1;
	int iaPlay = 1;
	int changeGameMode = 0;
	int (*selectedBoard)[9][9] = &defaultBoard;
	
	initEventManager(&myEM);
	addCallback(&myEM, &handle_EVENT, SDL_EVENT, &iaPlay);
	myWindow = buildWindow(&myEM, WINDOW_HEIGHT, WINDOW_WIDTH, "Abalone");
	tm = newTextureManager();
	spi = newSelectPlayerInterface(tm, myWindow->ren, &myEM);

	evalWeightsD.attack = 1000;
	evalWeightsD.defend = 10000;
	evalWeightsD.center = 100;
	evalWeightsD.grouping = 100;

	evalWeightsA.attack = 1000;
	evalWeightsA.defend = 10000;
	evalWeightsA.center = 100;
	evalWeightsA.grouping = 100;


	clock_t start;
	start = clock();

	while (mainEvent(&myEM) == 1 && cnt)
	{
		mainWindow(&myEM,myWindow);
		if (spi->gameMode == NO_GAME_MODE)
			drawSelectPlayerInterface(spi, &myEM);
		else if (changeGameMode == 0)
		{
			changeGameMode = 1;
			startGame(&ab, &myEM, myWindow->ren, tm, selectedBoard);
		}
		else 
			playGame(ab, &myEM, tm, spi->gameMode, &start, &iaPlay, &cnt, &evalWeightsA, &evalWeightsD);
	}

	if (cnt == 0)
		system("pause");

	if(ab!=NULL)
		deleteAdaloneBoard(ab);
	deleteTextureManager(tm);
	deleteWindow(myWindow);
	deleteEventManager(&myEM);

	return 0;
}