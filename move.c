#include <stdlib.h>
#include <assert.h>
#include "move.h"

move gen_move(board b, int square) {
    move m = malloc(sizeof(struct move));
    assert(m);
    int nb_moves = 0;
    int color = b->color[square];
    int piece = b->piece[square];
    int ray[5][8] = {
        {21, 19, 12, 8, -8 -12, -19, -21},
        {11, 9, -9, -11},
        {10, 1, -1, -10},
        {11, 10, 9, 1, -1 -9, -10, -11},
        {11, 10, 9, 1, -1 -9, -10, -11}
    };
    int piece_type;
    switch (piece) {
        case 1: // PAWN
            if (b->piece[square + 10 * color] == 7) {
                m->square[nb_moves] = square + 10 * color;
                nb_moves++;
            }
            if (b->color[square + 10 * color + 1] == color * -1) {
                m->square[nb_moves] = square + 10 * color + 1;
                nb_moves++;
            }
            if (b->color[square + 10 * color - 1] == color * -1) {
                m->square[nb_moves] = square + 10 * color - 1;
                nb_moves++;
            }
            return m;
        case 2: // KNIGHT
            piece_type = 0;
            for (int i = 0; i < 8; i++) {
                if (b->color[square + ray[piece_type][i]] == color * -1 || b->piece[square + ray[piece_type][i]] == 7) {
                    m->square[nb_moves] = square + ray[piece_type][i];
                    nb_moves++;
                }
            }
            return m;
        case 3: // BISHOP
            piece_type = 1;
            for (int i = 0; i < 4; i++) {
                int k = 1;
                while (b->piece[square + ray[piece_type][i] * k] != 0 && b->color[square + ray[piece_type][i] * k] != color) {
                    m->square[nb_moves] = square + ray[piece_type][i] * k;
                    nb_moves++;
                    if (b->piece[square + ray[piece_type][i] * k] == color * -1) {
                        break;
                    }
                    k++;
                }
            }
            return m;
        case 4: // ROOK
            piece_type = 2;
            for (int i = 0; i < 4; i++) {
                int k = 1;
                while (b->piece[square + ray[piece_type][i] * k] != 0 && b->color[square + ray[piece_type][i] * k] != color) {
                    m->square[nb_moves] = square + ray[piece_type][i] * k;
                    nb_moves++;
                    if (b->piece[square + ray[piece_type][i] * k] == color * -1) {
                        break;
                    }
                    k++;
                }
            }
            return m;
        case 5: // QUEEN
            piece_type = 3;
            for (int i = 0; i < 8; i++) {
                int k = 1;
                while (b->piece[square + ray[piece_type][i] * k] != 0 && b->color[square + ray[piece_type][i] * k] != color) {
                    m->square[nb_moves] = square + ray[piece_type][i] * k;
                    nb_moves++;
                    if (b->piece[square + ray[piece_type][i] * k] == color * -1) {
                        break;
                    }
                    k++;
                }
            }
            return m;
        case 6: // KING
            piece_type = 4;
            for (int i = 0; i < 8; i++) {
                if (b->color[square + ray[piece_type][i]] == color * -1 || b->piece[square + ray[piece_type][i]] == 7) {
                    m->square[nb_moves] = square + ray[piece_type][i];
                    nb_moves++;
                }
            }
            return m;
        default:
            return NULL;
    }
}
