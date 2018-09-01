#pragma once
#include "board.h"

typedef struct move {
    int square[30]; /* integers of possible squares */
} *move;

move gen_move(board b, int square);
