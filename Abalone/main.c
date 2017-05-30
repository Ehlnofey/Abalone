#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "EventManager.h"
#include "WindowManager.h"
#include "AbaloneBoard.h"
#include "minimax.h"


int handle_EVENT(void* handler, Event *e)
{
	int *iaPlay = (int*)(handler);
	SDL_Event *evt = (SDL_Event*)(e->data);

	if (evt->type == SDL_KEYDOWN)
		if (evt->key.keysym.sym == (SDL_Keycode)SDLK_p)
			*iaPlay = 0;

	return 0;
}

int main(int argc, char * argv[])
{
	EventManager myEM;
	BuildedWindow *myWindow;
	TextureManager *tm;
	AbaloneBoard *ab;
	EvalWeights evalWeightsD, evalWeightsA;
	int cnt = 1;
	int iaPlay = 1;
	
	initEventManager(&myEM);
	addCallback(&myEM, &handle_EVENT, SDL_EVENT, &iaPlay);
	myWindow = buildWindow(&myEM, WINDOW_HEIGHT, WINDOW_WIDTH, "Test !");
	tm = newTextureManager();
	ab = newAbaloneBoard(&myEM,myWindow->ren, tm, 14, 14);

	evalWeightsD.attack = 1000;
	evalWeightsD.defend = 10000;
	evalWeightsD.center = 100;
	evalWeightsD.grouping = 100;

	evalWeightsA.attack = 1000;
	evalWeightsA.defend = 10000;
	evalWeightsA.center = 100;
	evalWeightsA.grouping = 100;

	setDefaultConf(ab);

	clock_t start;
	start = clock();

	while (mainEvent(&myEM) == 1 && cnt)
	{
		mainWindow(&myEM,myWindow);
		setDrawableCoord(ab);
		drawBoard(ab, &myEM);
		if (clock() - start > 1 && iaPlay)
		{
			if (ab->turn == BLACK)
				start_ia(ab, &evalWeightsD, 3, 0);
			else
				start_ia(ab, &evalWeightsA, 3, 0);
			start = clock();
		}
		switch(someoneWin(ab))
		{
		case BLACK:
			cnt = 0;
			printf("Noir a gagne !\n");
			break;
		case WHITE:
			cnt = 0;
			printf("Blanc a gagne !\n");
			break;
		}
	}

	if (cnt == 0)
		system("pause");

	deleteAdaloneBoard(ab);
	deleteTextureManager(tm);
	deleteWindow(myWindow);
	deleteEventManager(&myEM);

	return 0;
}