#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "EventManager.h"
#include "WindowManager.h"
#include "AbaloneBoard.h"
#include "minimax.h"

int main(int argc, char * argv[])
{
	EventManager myEM;
	initEventManager(&myEM);
	BuildedWindow *myWindow;
	myWindow = buildWindow(&myEM, WINDOW_HEIGHT, WINDOW_WIDTH, "Test !");
	TextureManager *tm = newTextureManager();
	AbaloneBoard *ab = newAbaloneBoard(&myEM,myWindow->ren, tm, 14, 14);

	setDefaultConf(ab);

	clock_t start;
	start = clock();

	while (mainEvent(&myEM) == 1)
	{
		mainWindow(&myEM,myWindow);
		setDrawableCoord(ab);
		drawBoard(ab, &myEM);
		if (clock() - start > 100)
		{
			start_ia(ab,3);
			start = clock();
		}
	}

	deleteAdaloneBoard(ab);
	deleteTextureManager(tm);
	deleteWindow(myWindow);
	deleteEventManager(&myEM);

	return 0;
}