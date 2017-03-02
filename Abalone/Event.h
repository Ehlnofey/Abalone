#ifndef EVENT_H
#define EVENT_H

enum EVENT_CODE
{
	QUIT,DRAW,EVER, SDL_EVENT, NO_EVENT, IS_CLIKED
};

typedef struct _Event
{
	void *data;
	enum EVENT_CODE code;
	int haveToBeDelete;
}Event;

typedef struct _EventList
{
	struct _EventList *next;
	struct _EventList *previous;
	Event event;
}EventList;

void copyEvent(Event *orig, Event *dest);

#endif
