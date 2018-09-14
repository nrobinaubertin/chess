#pragma once
#include "board.h"

typedef struct move {
    int start;
    int end;
} *move;

move* create_move_list();
void destroy_move_list(move* ml);
move rand_move(board b, int color);
move* gen_move_list(board b, int square);
void apply_move(move m, board b);
