#include <stdlib.h>
#include "Event.h"
#include "WindowManager.h"

BuildedWindow* buildWindow(EventManager *em, int h, int w, char * windowName)
{
	SDL_Init(SDL_INIT_VIDEO);

	BuildedWindow *b = malloc(sizeof(BuildedWindow));

	b->win = SDL_CreateWindow(windowName, 100, 100, w, h, SDL_WINDOW_SHOWN);
	b->ren = SDL_CreateRenderer(b->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	addCallback(em, draw, DRAW,b);

	return b;
}

void deleteWindow(BuildedWindow *b)
{
	SDL_DestroyRenderer(b->ren);
	SDL_DestroyWindow(b->win);
	free(b); 
	IMG_Quit();
	SDL_Quit();
}


int draw(void *handler, Event *e)
{
	Drawable *drawable = (Drawable*)e->data;

	if (drawable->dst.w == -1 || drawable->dst.h == -1)
		SDL_QueryTexture(drawable->tex, NULL, NULL, &drawable->dst.w, &drawable->dst.h);

	SDL_RenderCopy(drawable->ren, drawable->tex, NULL, &drawable->dst);

	return 1;
}

void mainWindow(EventManager *em, BuildedWindow * b)
{
	Event evt;
	Event quitEvent;
	SDL_Event *e;

	evt.data = NULL;
	evt.code = NO_EVENT;
	evt.haveToBeDelete = 0;

	SDL_RenderPresent(b->ren);
	SDL_RenderClear(b->ren);

	pushEvent(evt);
	
	evt.code = SDL_EVENT;
	evt.haveToBeDelete = 1;
	evt.data = malloc(sizeof(SDL_Event));

	while (SDL_PollEvent(evt.data)) {

		e = evt.data;

		if (e->type == SDL_QUIT)
		{
			quitEvent.data = NULL;
			quitEvent.code = QUIT;
			quitEvent.haveToBeDelete = 0;

			pushEvent(quitEvent);
		}
		else
		{
			pushEvent(evt);
			evt.data = malloc(sizeof(SDL_Event));
		}
	}

	free(evt.data);
}

