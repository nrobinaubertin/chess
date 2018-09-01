#pragma once

typedef struct board {
    int color[120]; /* LIGHT = 1, DARK = -1, or EMPTY = 0 */
    int piece[120]; /* NOT USED, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY */
} *board;
board init_board(board b);
