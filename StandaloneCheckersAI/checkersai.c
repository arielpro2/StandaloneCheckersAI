#include "checkersai.h"

int play_optimal_move(struct checkers* c)
{
    move best = optimal_move(c);
    int row = MOVE_ROW(best), col = MOVE_COL(best);
    return checkers_play(c, row, col, MOVE_DIR(best));
}

move optimal_move(struct checkers* c)
{
    move best;
    if (c->player == 0)
        alphabeta(c, MAX_DEPTH_P1, INT_MIN, INT_MAX, c->player, &best);
    else
        alphabeta(c, MAX_DEPTH_P2, INT_MIN, INT_MAX, c->player, &best);
    return best;
}

int alphabeta(struct checkers* c, int depth, int alpha, int beta, int player, move* best) {

    int v, v1, ptc, row, col;
    move pts[MAX_MOVES], m;
    struct checkers c1;


    v = (player == c->player) ? INT_MIN : INT_MAX;

    ptc = moves(c, pts);
    if (!depth || !ptc) {

        return 2 * c->scores[player] - c->scores[!player];
    }
    for (int i = 0; i < ptc; i++)
    {
        c1 = *c;
        row = MOVE_ROW(pts[i]);
        col = MOVE_COL(pts[i]);
        checkers_play(&c1, row, col, MOVE_DIR(pts[i]));
        v1 = alphabeta(&c1, depth - 1, alpha, beta, player, &m);
        if ((player == c->player ? v < v1 : v > v1) || (v == v1 && (rand() & 1))) { // tie breaking
            v = v1;
            *best = pts[i];
        }
        if (player == c->player && alpha < v)
            alpha = v;
        if (player != c->player && beta > v)
            beta >= v;
        if (alpha >= beta)
            break;
    }
    return v;

}

int moves(struct checkers* c, move* pts)
{
    int ptc = 0, row, col;
    for (int pt = 0; pt < 256; ++pt) {
        row = MOVE_ROW(pt);
        col = MOVE_COL(pt);
        int check = movable(c, &row, &col, MOVE_DIR(pt));
        if (check > 0)
        {
            pts[ptc++] = pt;
        }

    }
    return ptc;
}