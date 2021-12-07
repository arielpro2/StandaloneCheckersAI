#include "checkers.h"
#include <limits.h>
#include <stdlib.h>

#define MAX_DEPTH_P1 10
#define MAX_DEPTH_P2 1

#define MAX_MOVES 28

typedef unsigned char move;
#define MOVE_ROW(m) ((m) >> 5)
#define MOVE_COL(m) ((m) >> 2 & 7)
#define MOVE_DIR(m) ((m) & 3)

int moves(struct checkers*, move*);

move optimal_move(struct checkers*);

int alphabeta(struct checkers*, int depth, int alpha, int beta, int player, move* best);

int play_optimal_move(struct checkers*);