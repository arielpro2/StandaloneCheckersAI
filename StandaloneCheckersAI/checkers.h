#define BOARD_SIZE 8
#define CheckOOB(row, col) ((row) & 8 || (col) & 8)

enum game_condition {
    P1_WIN, //0
    P2_WIN, //1
    NORMAL,
    TIE,
};

enum player {
    P1,
    P2
};

/* The values of the enum constants are determined such that
 * - The 0th bit determines whether the puck is occupied,
 * - The 1st bit determines whether it is occupied by player 1 or 2,
 * - The 2nd bit determines whether it is king, i.e. can move in both directions.
 */
enum puck {
    EMPTY, //000
    PL1, //001
    PL2 = 0b11,
    PL1KING = 0b101,
    PL2KING = 0b111
};

/* The 0th bit represents left or right,
   the 1st bit represents backward or forward movement */
enum dir {
    LEFT, //00
    RIGHT, //01
    BACKLEFT, //10
    BACKRIGHT //11
};

// different outcomes when moving
enum mov {
    OUTOFBOUNDS = -4,
    INVALID,
    DIRBLOCKED,
    REGULAR = 1,
    SCORED
};

struct checkers {
    unsigned char player,table[8][8];
    int turn, scores[2];
};

void checkers_init(struct checkers*);

int checkers_play(struct checkers*, int row, int col, int dir);

int movable(struct checkers*, int* row, int* col, int dir);

void print_board(struct checkers*);

int check_board(struct checkers*);