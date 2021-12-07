#include "checkers.h"
#include <stdio.h>
#include <windows.h>

int check_board(struct checkers* c) {
    unsigned char P1_PRESENCE = NULL;
    unsigned char P2_PRESENCE = NULL;
    unsigned char condition;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            unsigned char puck = c->table[row][col];
            if (!(puck & 1))
                continue;
            if (((puck & 2) >> 1))
                P2_PRESENCE = 1;
            else
                P1_PRESENCE = 1;
        }
    }
    if (P1_PRESENCE && !P2_PRESENCE)
        condition = P1_WIN;
    else
        condition = P2_WIN;
    if (P1_PRESENCE && P2_PRESENCE)
        condition = NORMAL;
    return condition;
}

void print_board(struct checkers* c)
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            printf("%d", c->table[row][col]);
        }
        printf("\n");
    }
}

void checkers_init(struct checkers* c)
{
    int i, j;
    if (!c)
        return;
    c->player = c->turn = c->scores[0] = c->scores[1] = 0;
    memset(c->table, 0, 64 * sizeof(unsigned char));
    for (i = 0; i < 3; ++i) {
        for (j = i & 1; j < 8; j += 2) {
            c->table[i][j] = PL1;
            c->table[7 - i][7 - j] = PL2;
        }
    }
}

int movable(struct checkers* c, int* row, int* col, int dir) {

    if (CheckOOB(*row, *col)) //check if puck out of bounds
        return OUTOFBOUNDS;

    unsigned char puck = c->table[*row][*col];
    if (!(puck & 1) || (((puck & 2) >> 1) ^ (c->player))) { //check if puck not occupied or occupied by another player
        return INVALID;
    }
    if (!(puck & 4) && dir & 2) { // check if puck is not king but moving backwards
        return DIRBLOCKED;
    }

    //int drow = BOOLTOSIGN(!c->player&!(dir&1) || c->player&(dir&1));
    //int dcol = BOOLTOSIGN(!c->player&!(dir&0) || c->player&(dir&0));
    //todo destination calc
    int drow = 0;
    int dcol = 0;
    if (c->player == P1) {
        switch (dir)
        {
        case LEFT:
            drow = 1;
            dcol = -1;
            break;
        case RIGHT:
            drow = 1;
            dcol = 1;
            break;
        case BACKLEFT:
            drow = -1;
            dcol = -1;
            break;
        case BACKRIGHT:
            drow = -1;
            dcol = 1;
            break;
        }
    }
    else if (c->player == P2) {
        switch (dir)
        {
        case LEFT:
            drow = -1;
            dcol = -1;
            break;
        case RIGHT:
            drow = -1;
            dcol = 1;
            break;
        case BACKLEFT:
            drow = 1;
            dcol = -1;
            break;
        case BACKRIGHT:
            drow = 1;
            dcol = 1;
            break;
        }
    }
    int row1 = *row + drow;
    int col1 = *col + dcol;
    if (CheckOOB(row1, col1)) // destination is out of bounds
        return OUTOFBOUNDS;

    if (c->table[row1][col1] & 1) {//new position occupied

        //can skip puck in new position (occupied by another player and the next position is not occupied and not out of bounds)
        if (!((row1 + drow) & 8) && !((col1 + dcol) & 8) &&
            (c->table[row1][col1] ^ puck) & 2 && !(c->table[row1 + drow][col1 + dcol] & 1)) {
            *row = row1 + drow;
            *col = col1 + dcol; //destination with skip
            return SCORED;
        }
        else {
            return DIRBLOCKED;
        }
    }

    *row = row1;
    *col = col1;//destination without skip

    return REGULAR;
}
int checkers_play(struct checkers* c, int row, int col, int dir) {
    int oldrow = row;
    int oldcol = col;
    int* drow = &row;
    int* dcol = &col;
    int code = movable(c, drow, dcol, dir);
    switch (code) //invalid play
    {
    case OUTOFBOUNDS:
        return OUTOFBOUNDS;
        break;
    case INVALID:
        return INVALID;
        break;
    case DIRBLOCKED:
        return DIRBLOCKED;
        break;
    }
    int oldpuck = c->table[oldrow][oldcol];
    c->table[oldrow][oldcol] = EMPTY;
    if (*drow == 7 && c->player == P1)
        c->table[*drow][*dcol] = PL1KING;
    else if (*drow == 0 && c->player == P2)
        c->table[*drow][*dcol] = PL2KING;
    else {
        c->table[*drow][*dcol] = oldpuck;

    }
    if (code == SCORED) {
        c->scores[c->player]++;
        c->turn++;
        c->table[(oldrow + *drow) / 2][(oldcol + *dcol) / 2] = EMPTY;
    }
    int i;
    int j;
    c->player = !c->player;
    return code;
}
