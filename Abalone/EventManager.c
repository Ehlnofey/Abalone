#include <stdlib.h>
#include "EventManager.h"
#include "CallbackList.h"

EventManager *getEventManager(EventManager *em)
{
	static EventManager *m_em = NULL;
	if (m_em == NULL)
		m_em = em;

	return m_em;
}

void initEventManager(EventManager *em)
{
	em->cl.code = -1;
	em->cl.handle = NULL;
	em->cl.next = NULL;
	em->cl.previous = NULL;

	em->el.event.code = -1;
	em->el.event.data = NULL;
	em->el.event.haveToBeDelete = 0;
	em->el.next = NULL;
	em->el.previous = NULL;

	getEventManager(em);
}

int mainEvent()
{
	EventManager *em = getEventManager(NULL);
	CallbackList *it = &em->cl;
	CallbackList *cbl = &em->cl;
	EventList *el = &em->el;
	int run = 1;

	while (it != NULL)
	{
		if (it->code == EVER)
			(*it->handle)(NULL);
		it = it->next;
	}
	while (el != NULL)
	{
		cbl = &em->cl;
		if (el->event.code == QUIT)
			run = 0;
		while (cbl != NULL)
		{
			if (cbl->handle != NULL && cbl->code == el->event.code)
			{
				if ((*cbl->handle)(&el->event) == 1)
					cbl = NULL;
				else
					cbl = cbl->next;
			}
			else if(cbl!=NULL)
				cbl = cbl->next;
		}
		if (el->event.haveToBeDelete == 1)
			free(el->event.data);
		el = el->next;
	}
	
	deleteEvent(em);

	return run;
}

void addCallback(int(*handle)(Event*e), enum EVENT_CODE code)
{
	EventManager *em = getEventManager(NULL);
	if (em->cl.handle == NULL)
	{
		em->cl.handle = handle;
		em->cl.code = code;
	}
	else
	{
		CallbackList *cl = malloc(sizeof(CallbackList));

		cl->handle = handle;
		cl->code = code;

		CallbackList *it = &em->cl;
		while (it->next != NULL)
			it = it->next;

		cl->previous = it;
		cl->next = NULL;
		it->next = cl;
	}
}

void pushEvent(Event e)
{
	EventManager *em = getEventManager(NULL);
	if (em->el.event.code == -1)
	{
		copyEvent(&e, &em->el.event);
	}
	else
	{
		EventList *el = malloc(sizeof(EventList));

		copyEvent(&e, &el->event);

		EventList *it = &em->el;

		while (it->next != NULL)
			it = it->next;

		el->previous = it;
		el->next = NULL;
		it->next = el;
	}
}

void deleteEvent()
{
	EventManager *em = getEventManager(NULL);
	EventList *it = em->el.next;
	while (it != NULL)
	{
		EventList *todelete = it;
		it = it->next;
		free(todelete);
	}
	em->el.next = NULL;
}

void deleteCallback()
{
	EventManager *em = getEventManager(NULL);
	CallbackList *it = em->cl.next;
	while (it != NULL)
	{
		CallbackList *todelete = it;
		it = it->next;
		free(todelete);
	}
	em->cl.next = NULL;
}

void deleteEventManager(EventManager *em)
{
	deleteCallback(em);
	deleteEvent(em);
}