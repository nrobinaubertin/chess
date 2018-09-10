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
    return b;
}

