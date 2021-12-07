#include "checkersai.h"

int main()
{
    struct checkers c;
    int count = 0;
    int condition = NORMAL;
    checkers_init(&c);
    while (condition == NORMAL) {
        play_optimal_move(&c);
        condition = check_board(&c);
        count++;
    }
    print_board(&c);
    printf("player no' %d won, took him %d turns!\n", condition+1, count);
}