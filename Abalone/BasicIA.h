#ifndef BASIC_IA_H
#define BASIC_IA_H

#include "AbaloneBoard.h"
#include "Tree.h"


void buildMovePossibility(TreeList *tl, AbaloneBoard *ab);
void addMove(TreeList *tl, AbaloneBoard *ab, int x, int y);
int workOutWeight(AbaloneBoard *ab, Move *m);

#endif // !BASIC_IA_H

