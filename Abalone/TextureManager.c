#include "TextureManager.h"
#include <string.h>

TextureManager * newTextureManager()
{
	TextureManager *t = malloc(sizeof(TextureManager));

	t->key = NULL;
	t->left = NULL;
	t->right = NULL;
	t->texture = NULL;

	return t;
}

SDL_Texture * getTexture(TextureManager *tm, char * name)
{
	int r = 0;
	if (tm == NULL)
		return NULL;
	if (tm->key == NULL)
		return NULL;

	r = strcmp(tm->key, name);

	if (r < 0)
		return getTexture(tm->left, name);
	else if (r > 0)
		return getTexture(tm->right, name);
	else
		return tm->texture;
}

Drawable getResizedDrawable(TextureManager *tm, SDL_Renderer *ren, char * name, int x, int y, int w, int h)
{
	Drawable d;

	d.dst.x = x;
	d.dst.y = y;
	d.dst.h = h;
	d.dst.w = w;

	d.ren = ren;

	d.tex = getTexture(tm, name);

	if (d.tex == NULL)
		d.tex = loadTexture(tm, ren, name);

	return d;
}

Drawable getDrawable(TextureManager *tm, SDL_Renderer *ren, char * name, int x, int y)
{
	return getResizedDrawable(tm, ren, name, x, y, -1, -1);
}

int insertTexture(SDL_Texture *texture,TextureManager ** tm, char * name)
{
	int r = 0;
	if (*tm == NULL)
	{
		*tm = newTextureManager();

		(*tm)->key = name;
		(*tm)->texture = texture;

		return 1;
	}
	else if ((*tm)->key == NULL)
	{
		(*tm)->key = name;
		(*tm)->texture = texture;

		return 1;
	}

	r = strcmp((*tm)->key, name);

	if (r < 0)
		return insertTexture(texture, &(*tm)->left, name);
	else if (r > 0)
		return insertTexture(texture, &(*tm)->right, name);
	else
		return 0;
}

SDL_Texture * loadTexture(TextureManager *tm, SDL_Renderer * ren, char * name)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, name);

	insertTexture(texture, &tm, name);

	return texture;
}

void deleteTextureManager(TextureManager *tm)
{
	if (tm != NULL)
	{
		deleteTextureManager(tm->left);
		deleteTextureManager(tm->right);
		SDL_DestroyTexture(tm->texture);
		free(tm);
	}
}
