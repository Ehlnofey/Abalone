#include "TreeList.h"

TreeList * newTreeList(Tree * t)
{
	TreeList *tl = malloc(sizeof(TreeList));
	tl->next = NULL;
	tl->t = t;
	return tl;
}

void deleteTreeList(TreeList * tl)
{
	free(tl);
}

void deepDeleteTreeList(TreeList * tl)
{
	TreeList *n = tl;
	while (n != NULL)
	{
		TreeList *c = n;
		n = n->next;
		deleteTreeList(c);
	}
}

void appendTreeList(TreeList * tl, Tree * t)
{
	TreeList *n = tl;
	while (n->next != NULL)
		n = n->next;
	n->next = newTreeList(t);
}
