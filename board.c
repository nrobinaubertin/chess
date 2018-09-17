#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "board.h"

board create_board() {
    board b = malloc(sizeof(struct board));
    assert(b);
    b->color = malloc(sizeof(int) * 120);
    assert(b->color);
    b->piece = malloc(sizeof(int) * 120);
    assert(b->piece);
    return b;
}

board copy_board(board b) {
    board bb = create_board();
    memcpy(bb->color, b->color, sizeof(int) * 120);
    memcpy(bb->piece, b->piece, sizeof(int) * 120);
    memcpy(bb->king_square, b->king_square, sizeof(int) * 2);
    memcpy(bb->castling_rights, b->castling_rights, sizeof(int) * 4);
    bb->who = b->who;
    return bb;
}

void destroy_board(board b) {
    if (b) {
        if (b->color)
            free(b->color);
        if (b->piece)
            free(b->piece);
        free(b);
    }
}

board init_board(board b) {
    memcpy(b->color, (int[]){
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, -1, -1, -1, -1, -1, -1, -1, -1, 0,
        0, -1, -1, -1, -1, -1, -1, -1, -1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }, sizeof(int) * 120);
    memcpy(b->piece, (int[]) {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 4, 2, 3, 5, 6, 3, 2, 4, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
        0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
        0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
        0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 4, 2, 3, 5, 6, 3, 2, 4, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }, sizeof(int) * 120);
    b->king_square[0] = 25;
    b->king_square[1] = 95;
    b->castling_rights[0] = true;
    b->castling_rights[1] = true;
    b->castling_rights[2] = true;
    b->castling_rights[3] = true;
    b->who = 1;
    return b;
}
