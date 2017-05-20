#include <stdio.h>
#include <assert.h>

#include "minIA.h"
int coups;

IA* new_ia(AbaloneBoard* ab) {
	int i, j;
	IA* ia;

	ia = (IA*)malloc(sizeof(IA));
	ia->nbBlackBalls = ia->nbWhiteBalls = 0;
	ia->turn = ab->turn;
	ia->pushed = 0;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (ab->board[i][j] == BLACK) {
				ia->nbBlackBalls++;
			}
			else if (ab->board[i][j] == WHITE) {
				ia->nbWhiteBalls++;
			}

			ia->board[i][j] = ab->board[i][j];
		}
	}

	return ia;
}

void copy_ia(IA* src, IA* dst) {
	int i, j;

	dst->turn = src->turn;
	dst->pushed = 0;
	dst->dx = 0;
	dst->dy = 0;
	dst->nbBlackBalls = src->nbBlackBalls;
	dst->nbWhiteBalls = src->nbWhiteBalls;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			dst->board[i][j] = src->board[i][j];
		}
	}
}

void free_ia(IA* ia) {
	free(ia);
}

AbaloneBoard* debugAB;

void min_ia_start(AbaloneBoard* ab) {
	IA* ia = new_ia(ab);
	debugAB = ab;

	coups = 0;
	printf("Best : %d\n", do_move(ia, 0, 3));
	printf("Coups : %d\n", coups);
	free_ia(ia);
}

void print_board(int board[SIZE][SIZE]) {
	int i, j;

	for (i = SIZE - 1; i >= 0; i--) {
		for (j = 0; j < SIZE; j++) {
			if (board[i][j] == OUT_ZONE) {
				printf("- ");
			}
			else if (board[i][j] == WHITE) {
				printf("%d ", 2);
			}
			else {
				printf("%d ", board[i][j]);
			}
		}

		printf("\n");
	}

	printf("-------------------\n");
}

void copy_board(int src[SIZE][SIZE], int dst[SIZE][SIZE]) {
	int i, j;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			dst[i][j] = src[i][j];
		}
	}
}

int calc_move(IA* ia, int startx, int starty, int max, int deep) {
	int i;
	IA* copy = (IA*)malloc(sizeof(IA));

	// Meilleur coup
	int best = (max) ? -10000 : 10000;
	int r;

	//print_board(ia->board);
	//printf("\n\n");

	for (i = 1; i <= 3; i++) {
		//if (i == 3) {
		// V
		copy_ia(ia, copy);
		if (can_move(copy, i, startx, starty, startx - 1, starty)) {
			//printf("coup -- \n");
			//print_board(copy->board);

			copy->board[startx - 1][starty] = copy->turn;
			copy->board[startx + i - 1][starty] = NO_BALL;

			if (copy->pushed > 0) {
				if (get_color(copy, startx - 1 - copy->pushed, starty) != OUT_ZONE) {
					copy->board[startx - 1 - copy->pushed][starty] = -(copy->turn);
				}
				else {
					if (copy->turn == WHITE) {
						copy->nbBlackBalls--;
					}
					else {
						copy->nbWhiteBalls--;
					}
				}
			}

			coups++;

			//print_board(copy->board);
			copy->turn = -(copy->turn);
			r = do_move(copy, !max, deep - 1);
			if (max && r > best || !max && r < best) {
				best = r;
			}
		}

		copy_ia(ia, copy);
		if (can_move(copy, i, startx + i - 1, starty, startx + i, starty)) {
			//printf("V%d:%d %d -> %d %d\n", i, startx + i - 1, starty, startx + i, starty);

			//printf("coup -- \n");
			//print_board(copy->board);

			copy->board[startx + i][starty] = copy->turn;
			copy->board[startx][starty] = NO_BALL;

			if (copy->pushed > 0) {
				if (get_color(copy, startx + i + copy->pushed, starty) != OUT_ZONE) {
					copy->board[startx + i + copy->pushed][starty] = -(copy->turn);
				}
				else {
					if (copy->turn == WHITE) {
						copy->nbBlackBalls--;
					}
					else {
						copy->nbWhiteBalls--;
					}
				}
			}

			coups++;

			//print_board(copy->board);
			copy->turn = -(copy->turn);
			r = do_move(copy, !max, deep - 1);
			if (max && r > best || !max && r < best) {
				best = r;
			}
		}

		// H
		copy_ia(ia, copy);
		if (can_move(copy, i, startx, starty + i - 1, startx, starty + i)) {
			//printf("H%d:%d %d -> %d %d\n", i, startx, starty + i - 1, startx, starty + i);
			copy->board[startx][starty + i] = copy->turn;
			copy->board[startx][starty] = NO_BALL;

			if (copy->pushed > 0) {
				if (get_color(copy, startx, starty + i + copy->pushed) != OUT_ZONE) {
					copy->board[startx][starty + i + copy->pushed] = -(copy->turn);
				}
				else {
					if (copy->turn == WHITE) {
						copy->nbBlackBalls--;
					}
					else {
						copy->nbWhiteBalls--;
					}
				}
			}

			coups++;

			//print_board(copy->board);
			copy->turn = -(copy->turn);
			r = do_move(copy, !max, deep - 1);
			if (max && r > best || !max && r < best) {
				best = r;
			}
		}
		copy_ia(ia, copy);
		if (can_move(copy, i, startx, starty, startx, starty - 1)) {
			//printf("H%d:%d %d -> %d %d\n", i, startx, starty, startx, starty - 1);
			copy->board[startx][starty - 1] = copy->turn;
			copy->board[startx][starty + i - 1] = NO_BALL;

			if (copy->pushed > 0) {
				if (get_color(copy, starty - 1 - copy->pushed, starty) != OUT_ZONE) {
					copy->board[startx][starty - 1 - copy->pushed] = -(copy->turn);
				}
				else {
					if (copy->turn == WHITE) {
						copy->nbBlackBalls--;
					}
					else {
						copy->nbWhiteBalls--;
					}
				}
			}

			coups++;

			//print_board(copy->board);
			copy->turn = -(copy->turn);
			r = do_move(copy, !max, deep - 1);
			if (max && r > best || !max && r < best) {
				best = r;
			}
		}

		// D
		copy_ia(ia, copy);
		if (can_move(copy, i, startx + i - 1, starty + i - 1, startx + i, starty + i)) {
			//printf("D%d:%d %d -> %d %d\n", i, startx + i - 1, starty + i - 1, startx + i, starty + i);
			copy->board[startx + i][starty + i] = copy->turn;
			copy->board[startx][starty] = NO_BALL;

			if (copy->pushed > 0) {
				if (get_color(copy, startx, starty + i + copy->pushed) != OUT_ZONE) {
					copy->board[startx + i + copy->pushed][starty + i + copy->pushed] = -(copy->turn);
				}
				else {
					if (copy->turn == WHITE) {
						copy->nbBlackBalls--;
					}
					else {
						copy->nbWhiteBalls--;
					}
				}
			}

			coups++;

			//print_board(copy->board);
			copy->turn = -(copy->turn);
			r = do_move(copy, !max, deep - 1);
			if (max && r > best || !max && r < best) {
				best = r;
			}
		}
		copy_ia(ia, copy);
		if (can_move(copy, i, startx, starty, startx - 1, starty - 1)) {
			//printf("D%d:%d %d -> %d %d\n", i, startx, starty, startx - 1, starty - 1);
			copy->board[startx - 1][starty - 1] = copy->turn;
			copy->board[startx + i - 1][starty + i - 1] = NO_BALL;

			if (copy->pushed > 0) {
				if (get_color(copy, startx, starty - 1 - copy->pushed) != OUT_ZONE) {
					copy->board[startx - 1 - copy->pushed][starty - 1 - copy->pushed] = -(copy->turn);
				}
				else {
					if (copy->turn == WHITE) {
						copy->nbBlackBalls--;
					}
					else {
						copy->nbWhiteBalls--;
					}
				}
			}

			coups++;

			//print_board(copy->board);
			copy->turn = -(copy->turn);
			r = do_move(copy, !max, deep - 1);
			if (max && r > best || !max && r < best) {
				best = r;
			}
		}
	}
	//}

	free_ia(copy);
	return best;
}

int do_move(IA* ia, int max, int deep) {
	if (deep == 0) {
		return eval(ia);
	}

	int i, j;
	int best = (max) ? -10000 : 10000;
	int r = best;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (ia->board[i][j] == ia->turn) {
				r = calc_move(ia, i, j, max, deep);
				if (max && r > best || !max && r < best) {
					best = r;
				}
			}
		}
	}

	return best;
}

int eval(IA* ia) {
	if (ia->turn = WHITE) {
		return ia->nbBlackBalls;
	}
	else {
		return ia->nbWhiteBalls;
	}
}

int can_move(IA* ia, unsigned short nb, int startx, int starty, int tox, int toy) {
	int can = 0;

	if (startx >= 0 && startx < SIZE && starty >= 0 && starty < SIZE && tox >= 0 && tox < SIZE && toy >= 0 && toy < SIZE && ia->board[startx][starty] == ia->turn) {
		int dir;

		if (startx == tox) {
			dir = toy - starty;
			ia->dx = 0;
			ia->dy = 1 * dir;
		}
		else if (starty == toy) {
			dir = tox - startx;
			ia->dx = 1 * dir;
			ia->dy = 0;
		}
		else if (startx == tox + 1 && starty == toy + 1) {
			dir = tox - startx;
			ia->dx = 1 * dir;
			ia->dy = 1 * dir;
		}

		if (nb == 1) {
			if (ia->board[tox][toy] == NO_BALL) {
				can = 1;
			}
		}
		else if (nb > 1) {
			int i;
			int align = 1;

			//if (ia->dx + ia->dy != 0) {
			for (i = 1; i < nb && align == 1; i++) {
				if (get_color(ia, startx - i * ia->dx, starty - i * ia->dy) != ia->turn) {
					align = 0;
				}
			}

			if (align) {
				if (ia->board[tox][toy] == NO_BALL) {
					can = 1;

				}
				else {
					if (nb > 1 && ia->board[tox][toy] == -(ia->turn)) {
						for (i = 1; i < nb && can == 0; i++) {
							int color = get_color(ia, tox + i * ia->dx, toy + i * ia->dy);

							if (color == NO_BALL || color == OUT_ZONE) {
								can = 1;
							}
							else if (color == -(ia->turn)) {
								ia->pushed++;
							}
						}

						if (can) {
							ia->pushed++;
						}
					}
				}
			}
			//}
		}
	}

	return can;
}

int get_color(IA* ia, int x, int y) {
	if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
		return ia->board[x][y];
	}
	else {
		return OUT_ZONE;
	}
}