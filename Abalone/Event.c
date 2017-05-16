#include "Event.h"

void copyEvent(Event * orig, Event * dest)
{
	dest->code = orig->code;
	dest->data = orig->data;
	dest->haveToBeDelete = orig->haveToBeDelete;
}
