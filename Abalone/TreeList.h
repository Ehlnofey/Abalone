#ifndef TREE_LIST_H
#define TREE_LIST_H

#include <stdlib.h>

typedef struct Tree Tree;

typedef struct TreeList
{
	struct TreeList* next;
	struct Tree* t;
}TreeList;

TreeList* newTreeList(Tree *t);//Allocat a new TreeList
void deleteTreeList(TreeList *tl); //free the tl pointer
void deepDeleteTreeList(TreeList *tl); //free all the list
void appendTreeList(TreeList *tl, Tree *t);

#endif // !TREE_LIST_H
