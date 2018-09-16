#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "move.h"
#include "print.h"

// A move list a an array of 100 moves.
// A move is a start (int) and an end (int)

static const int ray[5][5] = {
    {21, 19, 12, 8},
    {11, 9},
    {10, 1},
    {11, 10, 9, 1},
    {11, 10, 9, 1}
};

move* create_move_list() {
    move* ml = calloc(100, sizeof(struct move) * 100);
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
    for (int i = 0; i < 8; i++) {
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
                // destroy_move_list(ms);
            }
        }
    }
    move m = malloc(sizeof(struct move));
    assert(m);
    memcpy(m, ml[rand()%n], sizeof(struct move));
    destroy_move_list(ml);
    return m;
}

void add_move_to_ml(move* ml, int nb_moves, int start, int end) {
    move m = malloc(sizeof(struct move));
    m->start = start;
    m->end = end;
    ml[nb_moves] = m;
}

void add_ray_moves(board b, move* ml, int piece_type, int square) {
    int i = 0;
    int nb_moves = 0;
    int color = b->color[square];
    while(ray[piece_type][i] != 0) {
        for(int inv = -1; inv <= 1; inv += 2) {
            int k = 1;
            while (
                b->piece[square + inv*ray[piece_type][i] * k] != 0
                && b->color[square + inv*ray[piece_type][i] * k] != color
            ) {
                add_move_to_ml(ml, nb_moves, square, square + inv*ray[piece_type][i] * k);
                nb_moves++;
                if (b->piece[square + inv*ray[piece_type][i] * k] == color * -1) {
                    break;
                }
                k++;
            }
        }
        i++;
    }
}

void add_point_moves(board b, move* ml, int piece_type, int square) {
    int i = 0;
    int nb_moves = 0;
    int color = b->color[square];
    while(ray[piece_type][i] != 0) {
        for(int inv = -1; inv <= 1; inv += 2) {
            if (
                b->color[square + inv*ray[piece_type][i]] == color * -1
                || b->piece[square + inv*ray[piece_type][i]] == 7
            ) {
                add_move_to_ml(ml, nb_moves, square, square + inv*ray[piece_type][i]);
                nb_moves++;
            }
        }
        i++;
    }
}

move* gen_move_list(board b, int square) {
    move* ml = create_move_list();
    int piece = b->piece[square];
    int color = b->color[square];
    int nb_moves = 0;
    switch (piece) {
        case 1: // PAWN
            if (b->piece[square + 10 * color] == 7) {
                add_move_to_ml(ml, nb_moves, square, square + 10*color);
                nb_moves++;
                if (
                    (square/10 == 3 || square/10 == 8)
                    && b->piece[square + 20 * color] == 7
                ) {
                    add_move_to_ml(ml, nb_moves, square, square + 20*color);
                    nb_moves++;
                }
            }
            if (b->color[square + 10 * color + 1] == color * -1) {
                add_move_to_ml(ml, nb_moves, square, square + 10*color + 1);
                nb_moves++;
            }
            if (b->color[square + 10 * color - 1] == color * -1) {
                add_move_to_ml(ml, nb_moves, square, square + 10*color - 1);
                nb_moves++;
            }
            return ml;
        case 2: // KNIGHT
            add_point_moves(b, ml, 2, square);
            return ml;
        case 3: // BISHOP
            add_ray_moves(b, ml, 1, square);
            return ml;
        case 4: // ROOK
            add_ray_moves(b, ml, 2, square);
            return ml;
        case 5: // QUEEN
            add_ray_moves(b, ml, 3, square);
            return ml;
        case 6: // KING
            add_point_moves(b, ml, 4, square);
            return ml;
        default:
            return NULL;
    }
}

void apply_move(move m, board b) {
    b->color[m->end] = b->color[m->start];
    b->color[m->start] = 0;
    b->piece[m->end] = b->piece[m->start];
    b->piece[m->start] = 7;
    // pawn promotion to queen
    if (b->piece[m->end] == 1 && (m->end/10 == 2 || m->end/10 == 9))
        b->piece[m->end] = 5;
}
