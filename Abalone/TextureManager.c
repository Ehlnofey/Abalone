#include "TextureManager.h"
#include <string.h>

TextureManager * getTextureManager(TextureManager *tm);

TextureManager * newTextureManager()
{
	TextureManager *t = malloc(sizeof(TextureManager));

	t->key = NULL;
	t->left = NULL;
	t->right = NULL;
	t->texture = NULL;

	getTextureManager(t);

	return t;
}

SDL_Texture * getTextureFromRoot(char * name)
{
	return getTexture(getTextureManager(NULL),name);
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

Drawable getResizedDrawable(SDL_Renderer *ren, char * name, int x, int y, int w, int h)
{
	TextureManager *tm = getTextureManager(NULL);
	Drawable d;

	d.dst.x = x;
	d.dst.y = y;
	d.dst.h = h;
	d.dst.w = w;

	d.ren = ren;

	d.tex = getTexture(tm, name);

	if (d.tex == NULL)
		d.tex = loadTexture(ren, name);

	return d;
}

Drawable getDrawable(SDL_Renderer *ren, char * name, int x, int y)
{
	TextureManager *tm = getTextureManager(NULL);
	return getResizedDrawable(ren, name, x, y, -1, -1);
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

SDL_Texture * loadTexture(SDL_Renderer * ren, char * name)
{
	TextureManager *tm = getTextureManager(NULL);
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

TextureManager * getTextureManager(TextureManager *tm)
{
	static TextureManager *m_tm = NULL;
	if (m_tm == NULL)
		m_tm = tm;
	return m_tm;
}