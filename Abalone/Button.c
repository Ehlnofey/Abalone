#include "Button.h"

int drawableIsCliked(Drawable * d, SDL_Event * e)
{
	return e->button.x >= d->dst.x && e->button.y >= d->dst.y && e->button.x <= d->dst.x + +d->dst.w && e->button.y <= d->dst.y + d->dst.h;
}
