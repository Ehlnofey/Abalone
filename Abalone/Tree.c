#include "Tree.h"

Tree * newTree(Move * m, int weight)
{
	Tree *t = malloc(sizeof(Tree));
	t->me = *m;
	t->weight = weight;
	t->tl = newTreeList(NULL);

	return t;
}

void deleteTree(Tree * t)
{
	free(t);
}

void deepDeleteTree(Tree * t)
{
	TreeList *tl = t->tl;
	while (tl != NULL)
	{
		deepDeleteTree(tl->t);
		tl = tl->next;
	}

	deepDeleteTreeList(t->tl);

	deleteTree(t);
}
