#ifndef MINIMAX_THREADED_H
#define MINIMAX_THREADED_H

typedef struct MinimaxThreadStruct
{
	IA *ia;
	int deep;
	int max;
}MinimaxThreadStruct;

typedef struct ThreadList
{
	pthread_t id;
	Move value;
	struct ThreadList *next;
}ThreadList;

MinimaxThreadStruct* newMinimaxThreadStruct(IA *ia, int deep, int max);
BestMove *minimaxWithThread(IA* ia, int deep, int max);
ThreadList* newThreadList();
void deepDeleteThreadList(ThreadList * tl);
void* minimaxthreaded(void *arg);

#endif