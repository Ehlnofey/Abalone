#ifndef  TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
typedef struct Drawable
{
	SDL_Renderer *ren;
	SDL_Texture *tex;
	SDL_Rect dst;
}Drawable;


typedef struct TextureManager
{
	struct TextureManager *left,*right;
	char *key;
	SDL_Texture *texture;
}TextureManager;

TextureManager* newTextureManager();
SDL_Texture* getTextureFromRoot(char *name);
SDL_Texture* getTexture(TextureManager *tm, char *name);
Drawable getResizedDrawable(SDL_Renderer *ren, char *name, int x, int y, int w, int h);
Drawable getDrawable(SDL_Renderer *ren, char *name, int x, int y);
SDL_Texture* loadTexture(SDL_Renderer *ren,  char *name);
void deleteTextureManager(TextureManager *tm);

#endif // ! TEXTURE_MANAGER_H
