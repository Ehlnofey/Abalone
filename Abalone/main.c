#include <stdlib.h>
#include <stdio.h>
#include "EventManager.h"
#include "WindowManager.h"
#include "AbaloneBoard.h"

int main(int argc, char * argv[])
{
	EventManager myEM;
	initEventManager(&myEM);
	BuildedWindow *myWindow;
	myWindow = buildWindow(&myEM, 640, 800, "Test !");
	TextureManager *tm = newTextureManager();
	AbaloneBoard *ab = newAbaloneBoard(&myEM,myWindow->ren, tm, 14, 14);

	setDefaultConf(ab);

	while (mainEvent(&myEM) == 1)
	{
		mainWindow(&myEM,myWindow);
		setDrawableCoord(ab);
		drawBoard(ab, &myEM);
	}

	deleteAdaloneBoard(ab);
	deleteTextureManager(tm);
	deleteWindow(myWindow);
	deleteEventManager(&myEM);

	return 0;
}