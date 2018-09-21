#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "board.h"
#include "zobrist.h"

board create_board() {
    board b = malloc(sizeof(struct board));
    assert(b);
    b->color = calloc(120, sizeof(int));
    assert(b->color);
    b->piece = calloc(120, sizeof(int));
    assert(b->piece);
    return b;
}

bool boardcmp(board b1, board b2) {
    if (
        memcmp(b1->color, b2->color, sizeof(int) * 120) == 0
        && memcmp(b1->piece, b2->piece, sizeof(int) * 120) == 0
        && memcmp(b1->castling_rights, b2->castling_rights, sizeof(int) * 4) == 0
        && b1->who == b2->who
    ) {
        return true;
    }
    return false;
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

uint64_t hash_board(board b) {
    uint64_t hash = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            switch (b->color[k]) {
                case 1:
                    hash ^= hashpool[10 + 120 * (b->piece[k] - 1) + k];
                    break;
                case -1:
                    hash ^= hashpool[730 + 120 * (b->piece[k] - 1) + k];
                    break;
                default:
                    break;
            }
        }
    }
    for(int i = 0; i < 4; i++) {
        if (b->castling_rights[i])
            hash ^= hashpool[i];
    }
    if (b->who == 1)
        hash ^= hashpool[4];
    return hash;
}

uint64_t hash_piece(int color, int piece, int k) {
    if (color == 1) {
        return hashpool[10 + 120 * (piece - 1) + k];
    } else {
        return hashpool[730 + 120 * (piece - 1) + k];
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
    b->key = hash_board(b);
    return b;
}
