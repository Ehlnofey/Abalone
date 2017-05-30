#ifndef LIST_H
#define LIST_H

#include "Event.h"

typedef struct CallbackList
{
	struct CallbackList* next;
	struct CallbackList* previous;
	int(*handle)(void *, Event *);
	void *handler;
	enum EVENT_CODE code;
}CallbackList;

#endif // !LIST_H

