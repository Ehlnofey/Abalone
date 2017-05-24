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
} Move;

typedef struct MoveNode {
	struct MoveNode* next;
	Move* value;
} MoveNode;

typedef struct Ball {
	signed char onBoard;
	signed char x;
	signed char y;
} Ball;

typedef struct BestMove {
	int score;
	Move move;
} BestMove;

typedef struct EvalWeights {
	int attack;
	int defend;
	int center;
	int grouping;
} EvalWeights;

typedef struct IA {
	signed char board[SIZE][SIZE];
	signed char turn;
	Ball whiteBalls[NB_BALLS];
	Ball blackBalls[NB_BALLS];
	EvalWeights* evalWeights;
} IA;

IA* new_ia(AbaloneBoard* abalone, EvalWeights* evalWeights);
void copy_ia(IA* src, IA* dst);
void free_ia(IA* ia);
BestMove* minimax(IA* ia, int deep, int max);
void start_ia(AbaloneBoard* abalone, EvalWeights* evalWeights, int deep, int thread);
void print_board(IA* ia);
int eval(IA* ia);
signed char get(IA* ia, int x, int y);

extern signed char moveDirections[NB_MOVE_DIRECTIONS][2];
extern signed char selectionDirections[NB_SELECTION_DIRECTIONS][2];

MoveNode* selection(IA* ia);
void append_mono_ball_move(IA *ia, MoveNode** moves, signed char sx, signed char sy);
void append_possible_move(IA* ia, MoveNode** moves, signed char bx, signed char by, signed char sx, signed char sy, signed char nb);
int isLineMove(signed char sx, signed char sy, signed char mx, signed char my);
int possible_line_move(IA* ia, signed char bx, signed char by, signed char mx, signed char my, signed char nb);
int possible_broad_move(IA* ia, signed char bx, signed char by, signed char sx, signed char sy, signed char mx, signed char my, signed char nb);

void add_move(MoveNode** moves, signed char bx, signed char by, signed char sx, signed char sy, signed char mx, signed char my, signed char nb);
void free_moves(MoveNode* moves);
void perform_move(IA* ia, Move* move);

void move_ball(IA* ia, signed char bx, signed char by, signed char tox, signed char toy, int player);
void remove_ball(IA *ia, signed char bx, signed char by, int player);
Ball* get_balls(IA *ia, int player);
#endif