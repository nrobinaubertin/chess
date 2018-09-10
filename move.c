#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "move.h"

// A move list a an array of 100 moves.
// A move is a start (int) and an end (int)

move* create_move_list() {
    move* ml = malloc(sizeof(struct move) * 100);
    assert(ml);
    return ml;
}

void destroy_move_list(move* ml) {
    if (ml) {
        for (int i = 0; i < 100; i++) {
            if (ml[i])
                free(ml[i]);
        }
        free(ml);
    }
}

move rand_move(board b, int color) {
    move* ml = create_move_list();
    int n = 0;
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            if (b->color[k] == color) {
                move* ms = gen_move_list(b, k);
                if (ms) {
                    int it = 0;
                    while (ms[it]) {
                        ml[n] = ms[it];
                        it++;
                        n++;
                    }
                }
                destroy_move_list(ms);
            }
        }
    }
    move m = malloc(sizeof(struct move));
    memcpy(m, ml[rand()%n], sizeof(struct move));
    destroy_move_list(ml);
    return m;
}

move* gen_move_list(board b, int square) {
    move* ml = create_move_list();
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
                move m = malloc(sizeof(struct move));
                m->start = square;
                m->end = square + 10 * color;
                ml[nb_moves] = m;
                nb_moves++;
            }
            if (b->color[square + 10 * color + 1] == color * -1) {
                move m = malloc(sizeof(struct move));
                m->start = square;
                m->end = square + 10 * color + 1;
                ml[nb_moves] = m;
                nb_moves++;
            }
            if (b->color[square + 10 * color - 1] == color * -1) {
                move m = malloc(sizeof(struct move));
                m->start = square;
                m->end = square + 10 * color - 1;
                ml[nb_moves] = m;
                nb_moves++;
            }
            return ml;
        case 2: // KNIGHT
            piece_type = 0;
            for (int i = 0; i < 8; i++) {
                if (b->color[square + ray[piece_type][i]] == color * -1 || b->piece[square + ray[piece_type][i]] == 7) {
                    move m = malloc(sizeof(struct move));
                    m->start = square;
                    m->end = square + ray[piece_type][i];
                    ml[nb_moves] = m;
                    nb_moves++;
                }
            }
            return ml;
        case 3: // BISHOP
            piece_type = 1;
            for (int i = 0; i < 4; i++) {
                int k = 1;
                while (b->piece[square + ray[piece_type][i] * k] != 0 && b->color[square + ray[piece_type][i] * k] != color) {
                    move m = malloc(sizeof(struct move));
                    m->start = square;
                    m->end = square + ray[piece_type][i] * k;
                    ml[nb_moves] = m;
                    nb_moves++;
                    if (b->piece[square + ray[piece_type][i] * k] == color * -1) {
                        break;
                    }
                    k++;
                }
            }
            return ml;
        case 4: // ROOK
            piece_type = 2;
            for (int i = 0; i < 4; i++) {
                int k = 1;
                while (b->piece[square + ray[piece_type][i] * k] != 0 && b->color[square + ray[piece_type][i] * k] != color) {
                    move m = malloc(sizeof(struct move));
                    m->start = square;
                    m->end = square + ray[piece_type][i] * k;
                    ml[nb_moves] = m;
                    nb_moves++;
                    if (b->piece[square + ray[piece_type][i] * k] == color * -1) {
                        break;
                    }
                    k++;
                }
            }
            return ml;
        case 5: // QUEEN
            piece_type = 3;
            for (int i = 0; i < 8; i++) {
                int k = 1;
                while (b->piece[square + ray[piece_type][i] * k] != 0 && b->color[square + ray[piece_type][i] * k] != color) {
                    move m = malloc(sizeof(struct move));
                    m->start = square;
                    m->end = square + ray[piece_type][i] * k;
                    ml[nb_moves] = m;
                    nb_moves++;
                    if (b->piece[square + ray[piece_type][i] * k] == color * -1) {
                        break;
                    }
                    k++;
                }
            }
            return ml;
        case 6: // KING
            piece_type = 4;
            for (int i = 0; i < 8; i++) {
                if (b->color[square + ray[piece_type][i]] == color * -1 || b->piece[square + ray[piece_type][i]] == 7) {
                    move m = malloc(sizeof(struct move));
                    m->start = square;
                    m->end = square + ray[piece_type][i];
                    ml[nb_moves] = m;
                    nb_moves++;
                }
            }
            return ml;
        default:
            return NULL;
    }
}
