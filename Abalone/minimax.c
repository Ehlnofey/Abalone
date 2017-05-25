#define _TIMESPEC_DEFINED
#include <assert.h>
#include <pthread.h>
#include "minimax.h"
#include "MinimaxThreaded.h"

//#define MINIMAX_DEBUG

// Les 6 directions de déplacement possible
// Avec un axe x vers le haut et y vers la droite
signed char moveDirections[NB_MOVE_DIRECTIONS][2] = {
	{ 1,  0 }, // HAUT
	{ 1,  1 }, // HAUT DROITE
	{ 0,  1 }, // DROITE
	{ -1,  0 }, // BAS
	{ -1, -1 }, // BAS GAUCHE
	{ 0, -1 }  // GAUCHE
};

// Besoin que de 3 directions de sélection (pas les 6)
// Redondant dans pour l'algo de faire les 6
signed char selectionDirections[NB_SELECTION_DIRECTIONS][2] = {
	{ 1, 0 }, // VERTICAL
	{ 0, 1 }, // HORIZONTAL
	{ 1, 1 }  // DIAGONAL
};

IA* new_ia(AbaloneBoard* abalone, EvalWeights* evalWeights) {
	IA* ia = (IA*)malloc(sizeof(IA));

	assert(ia != NULL && evalWeights != NULL);

	int i, j, w, b;

	w = b = 0;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (abalone->board[i][j] == WHITE) {
				ia->whiteBalls[w].onBoard = 1;
				ia->whiteBalls[w].x = i;
				ia->whiteBalls[w].y = j;
				w++;
			}
			else if (abalone->board[i][j] == BLACK) {
				ia->blackBalls[b].onBoard = 1;
				ia->blackBalls[b].x = i;
				ia->blackBalls[b].y = j;
				b++;
			}

			ia->board[i][j] = abalone->board[i][j];
		}
	}

	for (i = w; i < NB_BALLS; i++) {
		ia->whiteBalls[i].onBoard = 0;
	}

	for (i = b; i < NB_BALLS; i++) {
		ia->blackBalls[i].onBoard = 0;
	}

	ia->turn = abalone->turn;
	ia->evalWeights = evalWeights;

	return ia;
}

MoveNode* new_moves() {
	MoveNode* moves = (MoveNode*)malloc(sizeof(MoveNode));

	return moves;
}

void free_moves(MoveNode* moves) {
	MoveNode* current = moves;

	while (current != NULL) {
		MoveNode* temp = current->next;
		free(current->value);
		free(current);

		current = temp;
	}
}

void free_ia(IA* ia) {
	free(ia);
}

signed char get(IA* ia, int x, int y) {
	if (x >= 0 && x < 9 && y >= 0 && y < 9) {
		return ia->board[x][y];
	}
	else {
		return OUT_ZONE;
	}
}

void copy_ia(IA* src, IA* dst) {
	int i, j;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			dst->board[i][j] = src->board[i][j];
		}
	}

	for (i = 0; i < NB_BALLS; i++) {
		dst->whiteBalls[i].x = src->whiteBalls[i].x;
		dst->whiteBalls[i].y = src->whiteBalls[i].y;
		dst->whiteBalls[i].onBoard = src->whiteBalls[i].onBoard;

		dst->blackBalls[i].x = src->blackBalls[i].x;
		dst->blackBalls[i].y = src->blackBalls[i].y;
		dst->blackBalls[i].onBoard = src->blackBalls[i].onBoard;
	}

	dst->evalWeights = src->evalWeights;
	dst->turn = src->turn;
}

int eval(IA* ia) {
	int i, j;

	j = 0;
	Ball* ball = get_balls(ia, ia->turn);

	for (i = 0; i < NB_BALLS; i++) {
		if (ball[i].onBoard) {
			j -= abs(ball[i].x - 4) + abs(ball[i].y - 4) * ia->evalWeights->center;

			if (
				get(ia, ball[i].x + 1, ball[i].y) == ia->turn
				&& get(ia, ball[i].x - 1, ball[i].y) == ia->turn
				&& get(ia, ball[i].x, ball[i].y + 1) == ia->turn
				&& get(ia, ball[i].x, ball[i].y - 1) == ia->turn
				&& get(ia, ball[i].x + 1, ball[i].y + 1) == ia->turn
				&& get(ia, ball[i].x - 1, ball[i].y - 1) == ia->turn) {

				j += ia->evalWeights->grouping;
			}
		}
		else {
			j -= ia->evalWeights->defend;
		}
	}

	ball = get_balls(ia, -(ia->turn));

	for (i = 0; i < NB_BALLS; i++) {
		if (ball[i].onBoard) {
			j += abs(ball[i].x - 4) + abs(ball[i].y - 4) * ia->evalWeights->center;
		}
		else {
			j += ia->evalWeights->attack;
		}
	}

	return j;
}

BestMove* minimax(IA* ia, int deep, int color, int alpha, int beta) {
	if (deep == 0) {
		BestMove *r = malloc(sizeof(BestMove));

		r->score = eval(ia) * color;
		return r;
	}
	else {
		IA* copy;
		BestMove* best = malloc(sizeof(BestMove));
		MoveNode* moves;
		MoveNode* current;

		copy = (IA*)malloc(sizeof(IA));
		assert(copy != NULL);

		best->score = -10000000;
		moves = selection(ia);
		current = moves;


		while (current != NULL) {
			copy_ia(ia, copy);
			perform_move(copy, current->value);
			copy->turn = -(copy->turn);
			BestMove* r = minimax(copy, deep - 1, -color, -beta, -alpha);
			r->score = -r->score;

			if (r->score > best->score) {
				best->score = r->score;
				best->move.bx = current->value->bx;
				best->move.by = current->value->by;
				best->move.mx = current->value->mx;
				best->move.my = current->value->my;
				best->move.sx = current->value->sx;
				best->move.sy = current->value->sy;
				best->move.nb = current->value->nb;

				if (r->score < alpha) {
					alpha = r->score;

					if (alpha >= beta) {
						free_ia(copy);
						free_moves(moves);
						free(r);

						return best;
					}
				}
			}

			free(r);
			current = current->next;
		}

		free_ia(copy);
		free_moves(moves);

		return best;
	}
}

void start_ia(AbaloneBoard* abalone, EvalWeights* evalWeight, int deep, int thread) {
	IA* ia = new_ia(abalone, evalWeight);

	BestMove* r;
	if (thread)
		r = minimaxWithThread(ia, deep, -1);
	else
		r = minimax(ia, deep, -1, -10000000, 10000000);

	printf("%d : (%c %d:%c %d) -> %c %d (%d)\n",
		r->score,
		r->move.bx + 65,
		r->move.by + 1,
		r->move.bx + (r->move.nb - 1) * r->move.sx + 65,
		r->move.by + (r->move.nb - 1) * r->move.sy + 1,
		r->move.bx + r->move.mx + 65,
		r->move.by + r->move.my + 1,
		r->move.nb
	);

	int i;

	for (i = 0; i < r->move.nb; i++) {
		isLeftCliked(abalone, r->move.bx + i * r->move.sx, r->move.by + i * r->move.sy);
	}

	printf("%c %d", r->move.bx + r->move.mx + 65, r->move.by + r->move.my + 1);

	isRightCliked(abalone, r->move.bx + r->move.mx, r->move.by + r->move.my);

	free_ia(ia);
	free(r);
}

void print_board(IA* ia) {
	int i, j;

	for (i = SIZE - 1; i >= 0; i--) {
		for (j = 0; j < SIZE; j++) {
			if (ia->board[i][j] == OUT_ZONE) {
				printf("  ");
			}
			else if (ia->board[i][j] == WHITE) {
				printf("2 ");
			}
			else {
				printf("%d ", ia->board[i][j]);
			}
		}

		printf("\n");
	}

	printf("\n\n");
}

MoveNode* selection(IA* ia) {
	int i;

	Ball* currentBalls = get_balls(ia, ia->turn);
	MoveNode* moves = NULL;

	for (i = 0; i < NB_BALLS; i++) {
		if (currentBalls[i].onBoard) {
			int j, k;

			// UNE BOULE
			append_mono_ball_move(ia, &moves, currentBalls[i].x, currentBalls[i].y);

			// DEUX ET TROIS BOULES
			for (j = 0; j < NB_SELECTION_DIRECTIONS; j++) {
				for (k = 1; k < 3; k++) {
					signed char v = get(ia, currentBalls[i].x + selectionDirections[j][0] * k, currentBalls[i].y + selectionDirections[j][1] * k);

					if (v == ia->turn) {
						append_possible_move(ia, &moves, currentBalls[i].x, currentBalls[i].y, selectionDirections[j][0], selectionDirections[j][1], k + 1);
					}
					// Contigu
					else {
						break;
					}
				}
			}
		}
	}

	return moves;
}

int isLineMove(signed char sx, signed char sy, signed char mx, signed char my) {
	if ((sx == 0 && mx == 0) || (sy == 0 && my == 0) || (sx == sy && mx == my)) {
		return 1;
	}

	return 0;
}

void append_mono_ball_move(IA *ia, MoveNode** moves, signed char sx, signed char sy) {
	int i;

	for (i = 0; i < NB_MOVE_DIRECTIONS; i++) {
		signed char v = get(ia, sx + moveDirections[i][0], sy + moveDirections[i][1]);

		if (v == NO_BALL) {
			add_move(moves, sx, sy, sx, sy, moveDirections[i][0], moveDirections[i][1], 1);
			//printf("  %c %d -> %c %d (1)\n", sx + 65, sy + 1, sx + moveDirections[i][0] + 65, sy + moveDirections[i][1] + 1);
		}
	}
}

int possible_line_move(IA* ia, signed char bx, signed char by, signed char mx, signed char my, signed char nb) {
	assert(nb > 1);

	signed char v1 = get(ia, bx + mx, by + my);

	if (v1 == OUT_ZONE || v1 == ia->turn) {
		return 0;
	}
	else if (v1 == NO_BALL) {
		return 1;
	}
	else {
		signed char v2 = get(ia, bx + mx * 2, by + my * 2);

		if (v2 == NO_BALL || v2 == OUT_ZONE) {
			return 1;
		}
		else if (nb > 2 && v2 != ia->turn) {
			signed char v3 = get(ia, bx + mx * 3, by + my * 3);

			if (v3 == NO_BALL || v3 == OUT_ZONE) {
				return 1;
			}
		}
	}

	return 0;
}

int possible_broad_move(IA* ia, signed char bx, signed char by, signed char sx, signed char sy, signed char mx, signed char my, signed char nb) {
	assert(nb > 1);

	int i, can;

	can = 1;

	for (i = 0; i < nb && can == 1; i++) {
		signed char v = get(ia, bx + sx * i + mx, by + sy * i + my);

		if (v != NO_BALL) {
			can = 0;
		}
	}

	return can;
}

void append_possible_move(IA* ia, MoveNode** moves, signed char bx, signed char by, signed char sx, signed char sy, signed char nb) {
	int i;

	for (i = 0; i < NB_MOVE_DIRECTIONS; i++) {
		int mx, my;
		int bLineMove;

		mx = moveDirections[i][0];
		my = moveDirections[i][1];
		bLineMove = isLineMove(sx, sy, mx, my);

		// LINE MOVE
		if (bLineMove) {
			int mx, my, nbx, nby;
			int invS;

			invS = 0;
			nbx = bx;
			nby = by;
			mx = moveDirections[i][0];
			my = moveDirections[i][1];

			if (mx == 1) {
				nbx = bx + (nb - 1) * sx;
				invS = 1;
			}
			if (my == 1) {
				nby = by + (nb - 1) * sy;
				invS = 1;
			}

			if (possible_line_move(ia, nbx, nby, mx, my, nb)) {
				if (invS) {
					add_move(moves, nbx, nby, -sx, -sy, mx, my, nb);
				}
				else {
					add_move(moves, nbx, nby, sx, sy, mx, my, nb);
				}
			}
		}
		else if (possible_broad_move(ia, bx, by, sx, sy, mx, my, nb)) {
			add_move(moves, bx, by, sx, sy, mx, my, nb);
		}
	}
}


void add_move(MoveNode** moves, signed char bx, signed char by, signed char sx, signed char sy, signed char mx, signed char my, signed char nb) {
	MoveNode* node = (MoveNode*)malloc(sizeof(MoveNode));

	assert(node != NULL);

	node->next = NULL;
	node->value = (Move*)malloc(sizeof(Move));

	assert(node->value != NULL);

	node->value->bx = bx;
	node->value->by = by;
	node->value->sx = sx;
	node->value->sy = sy;
	node->value->mx = mx;
	node->value->my = my;
	node->value->nb = nb;

	if (*moves == NULL) {
		*moves = node;
	}
	else {
		MoveNode* current = *moves;

		while (current->next != NULL) {
			current = current->next;
		}

		current->next = node;
	}
}

void perform_move(IA* ia, Move* move) {
	signed char tox = move->bx + move->mx;
	signed char toy = move->by + move->my;

	if (move->nb == 1) {
		ia->board[move->bx][move->by] = NO_BALL;
		ia->board[tox][toy] = ia->turn;

		move_ball(ia, move->bx, move->by, tox, toy, ia->turn);
	}
	else {
		if (!isLineMove(move->sx, move->sy, move->mx, move->my)) {
			int i;

			for (i = 0; i < move->nb; i++) {
				int fx, fy, tx, ty;

				fx = move->bx + i * move->sx;
				fy = move->by + i * move->sy;
				tx = move->bx + i * move->sx + move->mx;
				ty = move->by + i * move->sy + move->my;

				ia->board[fx][fy] = NO_BALL;
				ia->board[tx][ty] = ia->turn;

				move_ball(ia, fx, fy, tx, ty, ia->turn);
			}

#ifdef MINIMAX_DEBUG
			printf("broad move\n");
#endif // MINIMAX_DEBUG			
		}
		else {
			signed char lastbx = move->bx + move->sx * (move->nb - 1);
			signed char lastby = move->by + move->sy * (move->nb - 1);
			signed char v1 = get(ia, tox, toy);

			if (v1 == NO_BALL) {
				ia->board[lastbx][lastby] = NO_BALL;
				ia->board[tox][toy] = ia->turn;

				move_ball(ia, lastbx, lastby, tox, toy, ia->turn);
			}
			else {
				signed char v2 = get(ia, tox + move->mx, toy + move->my);

				if (v2 == NO_BALL) {
					ia->board[tox + move->mx][toy + move->my] = -ia->turn;
					ia->board[lastbx][lastby] = NO_BALL;
					ia->board[tox][toy] = ia->turn;

					move_ball(ia, tox, toy, tox + move->mx, toy + move->my, -(ia->turn));
					move_ball(ia, lastbx, lastby, tox, toy, ia->turn);
				}
				else if (v2 == OUT_ZONE) {
					ia->board[lastbx][lastby] = NO_BALL;
					ia->board[tox][toy] = ia->turn;

					move_ball(ia, lastbx, lastby, tox, toy, ia->turn);
					remove_ball(ia, tox, toy, -(ia->turn));
				}
				else {
					signed char v3 = get(ia, tox + move->mx * 2, toy + move->my * 2);

					if (v3 == NO_BALL) {
						ia->board[tox + move->mx * 2][toy + move->my * 2] = -(ia->turn);
						ia->board[lastbx][lastby] = NO_BALL;
						ia->board[tox][toy] = ia->turn;

						move_ball(ia, tox, toy, tox + move->mx * 2, toy + move->my * 2, -(ia->turn));
						move_ball(ia, lastbx, lastby, tox, toy, ia->turn);
					}
					else if (v3 == OUT_ZONE) {
						ia->board[lastbx][lastby] = NO_BALL;
						ia->board[tox][toy] = ia->turn;

						move_ball(ia, lastbx, lastby, tox, toy, ia->turn);
						remove_ball(ia, tox, toy, -(ia->turn));
					}
					else {
						printf("??");
					}
				}
			}
		}
	}
}

void move_ball(IA* ia, signed char bx, signed char by, signed char tox, signed char toy, int player) {
#ifdef MINIMAX_DEBUG
	printf("%c %d -> %c %d (%d) moved\n", bx + 65, by + 1, tox + 65, toy + 1, player);
#endif // MINIMAX_DEBUG


	Ball* current = get_balls(ia, player);
	int i;

	for (i = 0; i < NB_BALLS; i++) {
		if (current[i].x == bx && current[i].y == by) {
			current[i].x = tox;
			current[i].y = toy;
			break;
		}
	}
}

void remove_ball(IA *ia, signed char bx, signed char by, int player) {
#ifdef MINIMAX_DEBUG
	printf("%d %d (%d) removed\n", bx + 65, by + 1, player);
#endif // MINIMAX_DEBUG	

	Ball* current = get_balls(ia, player);
	int i;

	for (i = 0; i < NB_BALLS; i++) {
		if (current[i].x == bx && current[i].y == by) {
			current[i].onBoard = 0;
			break;
		}
	}
}

Ball* get_balls(IA *ia, int player) {
	if (player == WHITE) {
		return ia->whiteBalls;
	}
	else {
		return ia->blackBalls;
	}
}