#ifndef MIN_IA_H
#define MIN_IA_H
#define NB_BALLS 14

#include "AbaloneBoard.h"

typedef struct data {
	int r;
	int sx, sy, tx, ty;
} data;

typedef struct IA {
	int nbBlackBalls;
	int nbWhiteBalls;
	int board[SIZE][SIZE];
	int turn;
	int pushed;
	int dx;
	int dy;
} IA;

void copy_ia(IA* src, IA* dst);
IA* new_ia(AbaloneBoard* ab);
void free_ia(IA* ia);
void min_ia_start(AbaloneBoard* ab);
int can_move(IA* ia, unsigned short nb, int startx, int starty, int tox, int toy);
int do_move(IA* ia, int max, int deep);
int calc_move(IA* ia, int startx, int starty, int max, int deep);
int get_color(IA* ia, int x, int y);

int eval(IA* ia);

#endif // !MIN_IA_H