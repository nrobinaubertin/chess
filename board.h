#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct board {
    int* color; /* LIGHT = 1, DARK = -1, or EMPTY = 0 */
    int* piece; /* NOT USED, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY */
    int king_square[2];
    bool castling_rights[4];
    int who; // who's turn it is (color);
    uint64_t key; // hash
} *board;
board init_board(board b);
board create_board();
void destroy_board(board b);
board copy_board(board b);
uint64_t hash_board(board b);
bool boardcmp(board b1, board b2);
uint64_t hash_piece(int color, int piece, int k);
