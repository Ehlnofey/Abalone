#include <assert.h>
#include "minimax.h"

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


IA* new_ia(AbaloneBoard* abalone) {
	IA* ia = (IA*)malloc(sizeof(IA));
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
	ia->moves = NULL;

	if (ia->turn == WHITE) {
		ia->current = ia->blackBalls;
	}
	else {
		ia->current = ia->whiteBalls;
	}

	return ia;
}

void free_moves(IA* ia) {
	MoveNode* current = ia->moves;

	while (current != NULL) {
		MoveNode* temp = current->next;
		free(current->value);
		free(current);

		current = temp;
	}
}

void free_ia(IA* ia) {
	ia->current = NULL;
	free_moves(ia);
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

	dst->turn = src->turn;
	dst->current = NULL;
	dst->moves = NULL;
}

void play(AbaloneBoard* abalone) {
	IA* ia = new_ia(abalone);
	int i;

	for (i = 0; i < NB_BALLS; i++) {
		if (ia->whiteBalls[i].onBoard) {
			selection(ia, &ia->whiteBalls[i]);
		}
	}

	MoveNode* current = ia->moves;

	IA* copy = (IA*)malloc(sizeof(IA));
	
	while (current != NULL) {
		printf("(%c %d:%c %d) -> %c %d (%d)\n",
			current->value->bx + 65,
			current->value->by + 1,
			current->value->bx + (current->value->nb - 1) * current->value->sx + 65,
			current->value->by + (current->value->nb - 1) * current->value->sy + 1,
			current->value->bx + current->value->mx + 65,
			current->value->by + current->value->my + 1,
			current->value->nb);

		copy_ia(ia, copy);
		copy->current = copy->whiteBalls;
		perform_move(copy, current->value);
		print_board(copy);
		current = current->next;
	}

	free_ia(copy);
	free_ia(ia);
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

void selection(IA* ia, Ball* ball) {
	int i, j;

	// UNE BOULE
	append_mono_ball_move(ia, ball->x, ball->y);

	// DEUX ET TROIS BOULES
	for (i = 0; i < NB_SELECTION_DIRECTIONS; i++) {
		for (j = 1; j < 3; j++) {
			signed char v = get(ia, ball->x + selectionDirections[i][0] * j, ball->y + selectionDirections[i][1] * j);

			if (v == ia->turn) {
				append_possible_move(ia, ball->x, ball->y, selectionDirections[i][0], selectionDirections[i][1], j + 1);
			}
			// Contigu
			else {
				break;
			}
		}
	}
}

int isLineMove(signed char sx, signed char sy, signed char mx, signed char my) {
	if ((sx == 0 && mx == 0) || (sy == 0 && my == 0) || (sx == sy && mx == my)) {
		return 1;
	}

	return 0;
}

void append_mono_ball_move(IA *ia, signed char sx, signed char sy) {
	int i;

	for (i = 0; i < NB_MOVE_DIRECTIONS; i++) {
		signed char v = get(ia, sx + moveDirections[i][0], sy + moveDirections[i][1]);

		if (v == NO_BALL) {
			add_move(ia, sx, sy, sx, sy, moveDirections[i][0], moveDirections[i][1], 1);
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
	else if(v1 == NO_BALL) {
		return 1;
	}
	else {
		signed char v2 = get(ia, bx + mx * 2, by + my * 2);

		if (v2 == NO_BALL || v2 == OUT_ZONE) {
			return 1;
		}
		else if(nb > 2 && v2 != ia->turn) {
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

void append_possible_move(IA* ia, signed char bx, signed char by, signed char sx, signed char sy, signed char nb) {
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
					add_move(ia, nbx, nby, -sx, -sy, mx, my, nb);
				}
				else {
					add_move(ia, nbx, nby, sx, sy, mx, my, nb);
				}
			}
		}
		else if (possible_broad_move(ia, bx, by, sx, sy, mx, my, nb)) {
			add_move(ia, bx, by, sx, sy, mx, my, nb);
		}
	}
}


void add_move(IA* ia, signed char bx, signed char by, signed char sx, signed char sy, signed char mx, signed char my, signed char nb) {
	MoveNode* node = (MoveNode*)malloc(sizeof(MoveNode));

	node->next = NULL;
	node->value = (Move*)malloc(sizeof(Move));

	node->value->bx = bx;
	node->value->by = by;
	node->value->sx = sx;
	node->value->sy = sy;
	node->value->mx = mx;
	node->value->my = my;
	node->value->nb = nb;

	if (ia->moves == NULL) {
		ia->moves = node;
	}
	else {
		MoveNode* current = ia->moves;

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
	}
	else {
		if (!isLineMove(move->sx, move->sy, move->mx, move->my)) {
			int i;

			for (i = 0; i < move->nb; i++) {
				ia->board[move->bx + i * move->sx][move->by + i * move->sy] = NO_BALL;
				ia->board[move->bx + i * move->sx + move->mx][move->by + i * move->sy + move->my] = ia->turn;
			}

			printf("broad move\n");
			
		}
		else {
			signed char lastbx = move->bx + move->sx * (move->nb - 1);
			signed char lastby = move->by + move->sy * (move->nb - 1);
			signed char v1 = get(ia, tox, toy);

			if (v1 == NO_BALL) {
				ia->board[lastbx][lastby] = NO_BALL;
				ia->board[tox][toy] = ia->turn;
			}
			else {
				signed char v2 = get(ia, tox + move->mx * 2, toy + move->my * 2);

				if (v2 == NO_BALL) {
					ia->board[tox + move->mx * 2][tox + move->my * 2] = ia->board[tox][toy];
					ia->board[lastbx][lastby] = NO_BALL;
					ia->board[tox][toy] = ia->turn;
				}
				else if (v2 == OUT_ZONE) {
					ia->board[lastbx][lastby] = NO_BALL;
					ia->board[tox][toy] = ia->turn;

					Ball* current = ia->current;
					int i;

					for (i = 0; i < NB_BALLS; i++) {
						if (current[i].x == tox + move->mx * 2 && current[i].y == toy + move->my * 2) {
							current[i].onBoard = 0;
						}
					}
				}
				else {
					signed char v3 = get(ia, tox + move->mx * 3, toy + move->my * 3);

					if (v3 == NO_BALL) {
						ia->board[tox + move->mx * 3][toy + move->my * 3] = ia->board[tox][toy];
						ia->board[lastbx][lastby] = NO_BALL;
						ia->board[tox][toy] = ia->turn;
					}
					else if (v3 == OUT_ZONE) {
						ia->board[lastbx][lastby] = NO_BALL;
						ia->board[tox][toy] = ia->turn;

						Ball* current = ia->current;
						int i;

						for (i = 0; i < NB_BALLS; i++) {
							if (current[i].x == tox + move->mx * 3 && current[i].y == toy + move->my * 3) {
								current[i].onBoard = 0;
							}
						}
					}
					else {
						printf("??");
					}
				}
			}
		}
	}
}