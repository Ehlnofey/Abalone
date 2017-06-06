#define _TIMESPEC_DEFINED
#include <assert.h>
#include <pthread.h>
#include "minimax.h"
#include "MinimaxThreaded.h"

void* minimaxthreaded(void *arg)
{
#ifdef SHOW_TIME
	clock_t start;
#endif
	MinimaxThreadStruct *m = (MinimaxThreadStruct*)arg;

#ifdef SHOW_TIME
	start = clock();
#endif
	BestMove *b = minimax(m->ia, m->deep, m->max);
#ifdef SHOW_TIME
	printf("tmps fct thread : %dms\n", clock() - start);
#endif
	free(m->ia);
	free(m);
	pthread_exit(b);

	return b;
}
MinimaxThreadStruct * newMinimaxThreadStruct(IA * ia, int deep, int max)
{
	MinimaxThreadStruct* m = malloc(sizeof(MinimaxThreadStruct));
	m->deep = deep;
	m->max = max;
	m->ia = ia;
	return m;
}
ThreadList * newThreadList()
{
	ThreadList* tl = malloc(sizeof(ThreadList));
	tl->next = NULL;
	return tl;
}
void deepDeleteThreadList(ThreadList * tl)
{
	ThreadList *n = tl;
	while (n != NULL)
	{
		ThreadList *c = n;
		n = n->next;
		free(c);
	}
}

BestMove* minimaxWithThread(IA* ia, int deep, int color) {
	if (deep == 0 || get_nb_balls(ia, ia->turn) == 8 || get_nb_balls(ia, -ia->turn) == 8) {
		BestMove *r = malloc(sizeof(BestMove));

		r->score = eval(ia) * color;
		return r;
	}
	else {
		IA* copy;
		BestMove* best = malloc(sizeof(BestMove));
		MoveNode* moves;
		MoveNode* current;

		copy = (IA*)malloc(sizeof(IA));
		assert(copy != NULL);

		best->score = -10000000;
		moves = selection(ia);
		current = moves;

		ThreadList *threadIndex = NULL, *currentThread = NULL;

#ifdef SHOW_TIME
		clock_t start;
		start = clock();
#endif
		
		threadIndex = newThreadList();
		currentThread = threadIndex;
		
		while (current != NULL) {
			copy = (IA*)malloc(sizeof(IA));
			assert(copy != NULL);
			copy_ia(ia, copy);
			perform_move(copy, current->value);
			copy->turn = -(copy->turn);


			currentThread->next = newThreadList();
			currentThread = currentThread->next;
			currentThread->value = *current->value;

			pthread_create(&currentThread->id, NULL, minimaxthreaded, newMinimaxThreadStruct(copy, deep - 1, -color));

			//BestMove r = minimax(copy2, deep - 1, -color);

			current = current->next;
		}
#ifdef SHOW_TIME
		printf("tmps creations threads : %dms\n", clock() - start);
#endif
		currentThread = threadIndex->next;
		while (currentThread != NULL) {
			BestMove *b = NULL;
			pthread_join(currentThread->id, &b);
			b->score *= -1;
			if (b->score > best->score) {
				best->score = b->score;
				best->move = currentThread->value;
			}
			free(b);
			currentThread = currentThread->next;
		}

		deepDeleteThreadList(threadIndex);

		free_moves(moves);

		return best;
	}
}