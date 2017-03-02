#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "EventManager.h"
#include "TextureManager.h"


typedef struct BuildedWindow
{
	SDL_Window *win;
	SDL_Renderer *ren;
}BuildedWindow;

BuildedWindow* buildWindow(EventManager *em,int h, int w,char *windowName);
void deleteWindow(BuildedWindow *b);
int draw(Event *e);
void mainWindow(EventManager *em, BuildedWindow *b);

#endif // !WINDOW_MANAGER_H