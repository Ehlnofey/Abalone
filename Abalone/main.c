#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "EventManager.h"
#include "WindowManager.h"
#include "AbaloneBoard.h"
#include "minimax.h"
#include "SelectPlayerInterface.h"
#include "SelectBoardInterface.h"


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

void selectPlayerMenu(EventManager *em, SelectPlayerInterface *spi, int *changeGameMode)
{
	if (spi->gameMode == NO_GAME_MODE)
		drawSelectPlayerInterface(spi, em);
	else if (*changeGameMode == 0)
		*changeGameMode = 1;
}

void selectBoardMenu(EventManager *em, SelectBoardInterface *sbi, int *changeBoardType, int(**selectedBoard)[9][9])
{
	if (sbi->boardType == NO_BOARD_TYPE)
		drawSelectBoardInterface(sbi, em);
	else if (*changeBoardType == 0)
	{
		*changeBoardType = 1;

		switch (sbi->boardType)
		{
		case BELGIAN_DAISY_BOARD:
			*selectedBoard = &belgianDaisyBoard;
			break;
		default:
			*selectedBoard = &defaultBoard;
			break;
		}
	}
}

int main(int argc, char * argv[])
{
	EventManager myEM;
	BuildedWindow *myWindow;
	TextureManager *tm;
	AbaloneBoard *ab = NULL;
	SelectPlayerInterface *spi;
	SelectBoardInterface *sbi = NULL;
	EvalWeights evalWeightsD, evalWeightsA;
	int cnt = 1;
	int iaPlay = 1;
	int changeGameMode = 0, changeBoardType = 0;
	int (*selectedBoard)[9][9] = &belgianDaisyBoard;
	
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
		if (changeGameMode == 0)
		{
			selectPlayerMenu(&myEM, spi, &changeGameMode);
		}
		else if (changeBoardType == 0)
		{
			if(sbi==NULL)
				sbi = newSelectBoardInterface(tm, myWindow->ren, &myEM);

			selectBoardMenu(&myEM,sbi,&changeBoardType,&selectedBoard);
			if(changeBoardType==1)
				startGame(&ab, &myEM, myWindow->ren, tm, selectedBoard);
		}
		else 
			playGame(ab, &myEM, tm, spi->gameMode, &start, &iaPlay, &cnt, &evalWeightsD, &evalWeightsA);
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