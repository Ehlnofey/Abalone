#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "CallbackList.h"
#include "Event.h"

typedef struct EventManager
{
	EventList el;
	CallbackList cl;
}EventManager;

void initEventManager(EventManager *em);
int mainEvent(EventManager *em);
void addCallback(EventManager *em, int(*handle)(Event*e), enum EVENT_CODE code);
void pushEvent(Event e);
void deleteEvent(EventManager *em);
void deleteCallback(EventManager *em);
void deleteEventManager(EventManager *em);

#endif // !EVENT_MANAGER_H

