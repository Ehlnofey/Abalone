#ifndef BASIC_IA_H
#define BASIC_IA_H

#include "AbaloneBoard.h"
#include "Tree.h"

typedef struct  BasicIA
{
	Tree *t;
	int color;
}BasicIA;

BasicIA* newBasicIA(int color);
void deleteBasicIA(BasicIA *ia);
void buildMovePossibility(TreeList *tl, AbaloneBoard *ab);
void makeFloor(TreeList *tl, AbaloneBoard *ab);
void addMove(TreeList *tl, AbaloneBoard *ab, int x, int y);
int workOutWeight(AbaloneBoard *ab, Move *m);
void playMove(Move *m, AbaloneBoard *ab);
void unPlayMove(Move *m, AbaloneBoard *ab);

#endif // !BASIC_IA_H

