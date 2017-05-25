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
	BestMove *b = minimax(m->ia, m->deep, m->max, -10000000, 10000000);
#ifdef SHOW_TIME
	printf("tmps fct thread : %dms\n", clock() - start);
#endif
	free(m);

	pthread_exit(b);

	return NULL;
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
	tl->b = NULL;
	tl->next = NULL;
	tl->value = NULL;
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

BestMove* minimaxWithThread(IA* ia, int deep, int max) {
	if (deep == 0) {
		BestMove *r = malloc(sizeof(BestMove));

		r->score = eval(ia);
		return r;
	}
	else {
		IA* copy;
		BestMove* best = malloc(sizeof(BestMove));
		MoveNode* moves;
		MoveNode* current;

		copy = (IA*)malloc(sizeof(IA));
		assert(copy != NULL);

		best->score = (max) ? -10000000 : 10000000;
		moves = selection(ia);
		current = moves;

		ThreadList *threadIndex = NULL, *currentThread = NULL;

#ifdef SHOW_TIME
		clock_t start;
		start = clock();
#endif
		while (current != NULL) {
			copy_ia(ia, copy);
			perform_move(copy, current->value);
			copy->turn = -(copy->turn);

			if (threadIndex == NULL)
			{
				threadIndex = newThreadList();
				currentThread = threadIndex;
			}

			currentThread->next = newThreadList();
			currentThread = currentThread->next;
			currentThread->value = current->value;

			pthread_create(&currentThread->id, NULL, minimaxthreaded, newMinimaxThreadStruct(copy, deep - 1, !max));

			//BestMove r = minimax(copy2, deep - 1, !max);

			current = current->next;
		}
#ifdef SHOW_TIME
		printf("tmps creations threads : %dms\n", clock() - start);
#endif
		currentThread = threadIndex->next;
		while (currentThread != NULL) {
			pthread_join(currentThread->id, &currentThread->b);
			if (max && currentThread->b->score > best->score || !max && currentThread->b->score < best->score) {
				best->score = currentThread->b->score;
				best->move.bx = currentThread->value->bx;
				best->move.by = currentThread->value->by;
				best->move.mx = currentThread->value->mx;
				best->move.my = currentThread->value->my;
				best->move.sx = currentThread->value->sx;
				best->move.sy = currentThread->value->sy;
				best->move.nb = currentThread->value->nb;
			}
			free(currentThread->b);
			currentThread = currentThread->next;
		}

		deepDeleteThreadList(threadIndex);


		free_ia(copy);
		free_moves(moves);

		return best;
	}
}