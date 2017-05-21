#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdio.h>
#include "AbaloneBoard.h"

#define NB_BALLS 14
#define NB_MOVE_DIRECTIONS 6
#define NB_SELECTION_DIRECTIONS 3

typedef struct Move {
	signed char bx;
	signed char by;
	signed char sx;
	signed char sy;
	signed char mx;
	signed char my;
	signed char nb;
	signed char isBroad;
} Move;

typedef struct MoveNode {
	struct MoveNode* next;
	Move *value;
} MoveNode;

typedef struct Ball {
	signed char onBoard;
	signed char x;
	signed char y;
} Ball;

typedef struct IA {
	signed char board[SIZE][SIZE];
	signed char turn;
	MoveNode *moves;
	Ball whiteBalls[NB_BALLS];
	Ball blackBalls[NB_BALLS];
	Ball *current;
} IA;

IA* new_ia(AbaloneBoard* abalone);
void copy_ia(IA* src, IA* dst);
void free_ia(IA* ia);
void play(AbaloneBoard* abalone);
void print_board(IA* ia);
signed char get(IA* ia, int x, int y);

extern signed char moveDirections[NB_MOVE_DIRECTIONS][2];
extern signed char selectionDirections[NB_SELECTION_DIRECTIONS][2];

void selection(IA* ia, Ball* ball);
void append_mono_ball_move(IA *ia, signed char sx, signed char sy);
void append_possible_move(IA* ia, signed char bx, signed char by, signed char sx, signed char sy, signed char nb);
int isLineMove(signed char sx, signed char sy, signed char mx, signed char my);
int possible_line_move(IA* ia, signed char bx, signed char by, signed char mx, signed char my, signed char nb);
int possible_broad_move(IA* ia, signed char bx, signed char by, signed char sx, signed char sy, signed char mx, signed char my, signed char nb);

void add_move(IA* ia, signed char bx, signed char by, signed char sx, signed char sy, signed char mx, signed char my, signed char nb);
void free_moves(IA* ia);
void perform_move(IA* ia, Move* move);
#endif