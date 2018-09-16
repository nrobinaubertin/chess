#pragma once
#include <stdbool.h>

typedef struct board {
    int* color; /* LIGHT = 1, DARK = -1, or EMPTY = 0 */
    int* piece; /* NOT USED, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY */
    int king_square[2];
    bool castling_rights[4];
} *board;
board init_board(board b);
board create_board();
void destroy_board(board b);
board copy_board(board b);
