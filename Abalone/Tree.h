#ifndef TREE_H
#define TREE_H

#include "TreeList.h"

typedef struct Move
{
	int originx, originy;
	int destx, desty;
}Move;

typedef struct Tree
{
	Move me;
	int weight;
	struct Tree *tree_min, *tree_max;
	struct TreeList *tl;
}Tree;

Tree* newTree(Move *m, int weight);//Allocat a new Tree 
void deleteTree(Tree *t);//only free t pointer
void deepDeleteTree(Tree *t);//free all tree in t->tl, free t->tl and free t;

#endif // !TREE_H