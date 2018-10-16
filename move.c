#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "move.h"
#include "print.h"
#include "zobrist.h"

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
    return ml;
}

move copy_move(move m) {
    move mm = malloc(sizeof(struct move));
    mm->start = m->start;
    mm->end = m->end;
    return mm;
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

move* gen_all_moves(board b) {
    int color = b->who;
    move* ml = create_move_list();
    int n = add_castle_moves(b, ml, 0, color);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            if (b->color[k] == color) {
                n = add_move_list(b, k, ml, n);
            }
        }
    }
    return ml;
}

move rand_move(board b) {
    int color = b->who;
    move* ml = create_move_list();
    int n = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            if (b->color[k] == color) {
                n = add_move_list(b, k, ml, n);
            }
        }
    }
    n = add_castle_moves(b, ml, n, color);
    move m = malloc(sizeof(struct move));
    memcpy(m, ml[rand()%n], sizeof(struct move));
    destroy_move_list(ml);
    return m;
}

void add_move_to_ml(move* ml, int n, int start, int end) {
    move m = malloc(sizeof(struct move));
    m->start = start;
    m->end = end;
    ml[n] = m;
}

int add_ray_moves(board b, move* ml, int n, int piece_type, int square, int color) {
    int i = 0;
    while(ray[piece_type][i] != 0) {
        for(int inv = -1; inv <= 1; inv += 2) {
            int k = 1;
            while (
                b->piece[square + inv*ray[piece_type][i] * k] == 7
                || b->color[square + inv*ray[piece_type][i] * k] == color * -1
            ) {
                add_move_to_ml(ml, n, square, square + inv*ray[piece_type][i] * k);
                n++;
                // if we eat an enemy piece, end the ray
                if (b->color[square + inv*ray[piece_type][i] * k] == color * -1) {
                    break;
                }
                k++;
            }
        }
        i++;
    }
    return n;
}

int add_point_moves(board b, move* ml, int n, int piece_type, int square, int color) {
    int i = 0;
    while(ray[piece_type][i] != 0) {
        for(int inv = -1; inv <= 1; inv += 2) {
            if (
                b->piece[square + inv*ray[piece_type][i]] == 7
                || b->color[square + inv*ray[piece_type][i]] == color * -1
            ) {
                add_move_to_ml(ml, n, square, square + inv*ray[piece_type][i]);
                n++;
            }
        }
        i++;
    }
    return n;
}

int add_move_list(board b, int square, move* ml, int n) {
    int piece = b->piece[square];
    int color = b->color[square];
    switch (piece) {
        case 1: // PAWN
            if (b->piece[square + 10 * color] == 7) {
                add_move_to_ml(ml, n, square, square + 10*color);
                n++;
                if (
                    (square/10 == 3 || square/10 == 8)
                    && b->piece[square + 20 * color] == 7
                ) {
                    add_move_to_ml(ml, n, square, square + 20*color);
                    n++;
                }
            }
            if (b->color[square + 10 * color + 1] == color * -1) {
                add_move_to_ml(ml, n, square, square + 10*color + 1);
                n++;
            }
            if (b->color[square + 10 * color - 1] == color * -1) {
                add_move_to_ml(ml, n, square, square + 10*color - 1);
                n++;
            }
            return n;
        case 2: // KNIGHT
            n = add_point_moves(b, ml, n, 0, square, color);
            return n;
        case 3: // BISHOP
            n= add_ray_moves(b, ml, n, 1, square, color);
            return n;
        case 4: // ROOK
            n = add_ray_moves(b, ml, n, 2, square, color);
            return n;
        case 5: // QUEEN
            n = add_ray_moves(b, ml, n, 3, square, color);
            return n;
        case 6: // KING
            n = add_point_moves(b, ml, n, 4, square, color);
            return n;
        default:
            return n;
    }
}

int add_castle_moves(board b, move* ml, int nb_moves, int color) {
    if (color == 1) {
        if (
            b->castling_rights[1]
            && b->color[22] == 0
            && b->color[23] == 0
            && b->color[24] == 0
            && !is_square_checked(b, 1, 25)
            && !is_square_checked(b, 1, 24)
            && !is_square_checked(b, 1, 23)
        ) {
            add_move_to_ml(ml, nb_moves, b->king_square[0], 101);
            nb_moves++;
        }
        if (
            b->castling_rights[0]
            && b->color[27] == 0
            && b->color[26] == 0
            && !is_square_checked(b, 1, 25)
            && !is_square_checked(b, 1, 26)
            && !is_square_checked(b, 1, 27)
        ) {
            add_move_to_ml(ml, nb_moves, b->king_square[0], 100);
            nb_moves++;
        }
    } else {
        if (
            b->castling_rights[3]
            && b->color[92] == 0
            && b->color[93] == 0
            && b->color[94] == 0
            && !is_square_checked(b, -1, 95)
            && !is_square_checked(b, -1, 94)
            && !is_square_checked(b, -1, 93)
        ) {
            add_move_to_ml(ml, nb_moves, b->king_square[0], 103);
            nb_moves++;
        }
        if (
            b->castling_rights[2]
            && b->color[97] == 0
            && b->color[96] == 0
            && !is_square_checked(b, 1, 95)
            && !is_square_checked(b, 1, 96)
            && !is_square_checked(b, 1, 97)
        ) {
            add_move_to_ml(ml, nb_moves, b->king_square[0], 102);
            nb_moves++;
        }
    }
    return nb_moves;
}

bool is_king_checked(board b, int color) {
    if (color == 1)
        return is_square_checked(b, color, b->king_square[0]);
    return is_square_checked(b, color, b->king_square[1]);
}

// this function places a virtual king of the chosen color of the chose square
// and tests if he is in check
bool is_square_checked(board b, int color, int square) {
    // remove the king (it can block rays)
    if (color == 1) {
        b->piece[b->king_square[0]] = 7;
        b->color[b->king_square[0]] = 0;
    } else {
        b->piece[b->king_square[1]] = 7;
        b->color[b->king_square[1]] = 0;
    }
    // 1. check if a knight threatens the square
    for(int i = 0; i < 4; i++) {
        for(int inv = -1; inv <= 1; inv += 2) {
            if (b->piece[inv*ray[0][i]] == 2 && b->color[inv*ray[0][i]] == color*-1) {
                // re-add the king
                if (color == 1) {
                    b->piece[b->king_square[0]] = 6;
                    b->color[b->king_square[0]] = 1;
                } else {
                    b->piece[b->king_square[1]] = 6;
                    b->color[b->king_square[1]] = -1;
                }
                return true;
            }
        }
    }
    // 2. ray-check in cardinal directions for an enemy piece
    for(int i = 0; i < 2; i++) {
        for(int inv = -1; inv <= 1; inv += 2) {
            int k = 1;
            while (b->piece[square + inv*ray[2][i] * k] == 7) {
                k++;
            }
            if (
                b->color[square + inv*ray[2][i] * k] == color*-1
                && (
                    b->piece[square + inv*ray[2][i] * k] == 4
                    || b->piece[square + inv*ray[2][i] * k] == 5
                )
            ) {
                // re-add the king
                if (color == 1) {
                    b->piece[b->king_square[0]] = 6;
                    b->color[b->king_square[0]] = 1;
                } else {
                    b->piece[b->king_square[1]] = 6;
                    b->color[b->king_square[1]] = -1;
                }
                return true;
            }
        }
    }
    // 3. ray-check in diagonal directions for an enemy piece
    for(int i = 0; i < 2; i++) {
        for(int inv = -1; inv <= 1; inv += 2) {
            int k = 1;
            // special check for pawns
            if (
                inv == color
                && b->color[square + inv*ray[2][i]] == color*-1
                && b->piece[square + inv*ray[2][i]] == 1
            ) {
                // re-add the king
                if (color == 1) {
                    b->piece[b->king_square[0]] = 6;
                    b->color[b->king_square[0]] = 1;
                } else {
                    b->piece[b->king_square[1]] = 6;
                    b->color[b->king_square[1]] = -1;
                }
                return true;
            }
            while (b->piece[square + inv*ray[2][i] * k] == 7) {
                k++;
            }
            if (
                b->color[square + inv*ray[2][i] * k] == color*-1
                && (
                    b->piece[square + inv*ray[2][i] * k] == 3
                    || b->piece[square + inv*ray[2][i] * k] == 5
                )
            ) {
                // re-add the king
                if (color == 1) {
                    b->piece[b->king_square[0]] = 6;
                    b->color[b->king_square[0]] = 1;
                } else {
                    b->piece[b->king_square[1]] = 6;
                    b->color[b->king_square[1]] = -1;
                }
                return true;
            }
        }
    }
    // re-add the king
    if (color == 1) {
        b->piece[b->king_square[0]] = 6;
        b->color[b->king_square[0]] = 1;
    } else {
        b->piece[b->king_square[1]] = 6;
        b->color[b->king_square[1]] = -1;
    }
    return false;
}

void atomic_move(board b, int start, int end) {
    // remove moving piece from the key
    b->key ^= hash_piece(b->color[start], b->piece[start], start);
    // remove the eventual taken piece from the key
    if (b->color[end] != 0) {
        b->key ^= hash_piece(b->color[end], b->piece[end], end);
    }
    b->color[end] = b->color[start];
    b->piece[end] = b->piece[start];
    b->color[start] = 0;
    b->piece[start] = 7;
    // pawn promotion to queen
    if (b->piece[end] == 1 && (end/10 == 2 || end/10 == 9))
        b->piece[end] = 5;
    // add moving piece to the key
    b->key ^= hash_piece(b->color[end], b->piece[end], end);
}

void change_castling_rights(board b, int castle, bool value) {
    b->castling_rights[castle] = value;
    b->key ^= hashpool[castle];
}

void apply_move(move m, board b) {

    // add current key to the board keys_history
    int i = 0;
    while (b->keys_history[i] != 0)
        i++;
    b->keys_history[i] = b->key;

    // is it a castle move ?
    switch (m->end) {
        default:
            break;
        case 100:
            atomic_move(b, 25, 27);
            atomic_move(b, 28, 26);
            return;
        case 101:
            atomic_move(b, 25, 23);
            atomic_move(b, 21, 24);
            return;
        case 102:
            atomic_move(b, 95, 97);
            atomic_move(b, 98, 96);
            return;
        case 103:
            atomic_move(b, 95, 93);
            atomic_move(b, 91, 94);
            return;
    }

    atomic_move(b, m->start, m->end);

    // special cases
    switch (b->piece[m->end]) {
        case 6:
            // king move
            if (b->color[m->end] == 1) {
                b->king_square[0] = m->end;
                change_castling_rights(b, 0, false);
                change_castling_rights(b, 1, false);
            } else {
                b->king_square[1] = m->end;
                change_castling_rights(b, 2, false);
                change_castling_rights(b, 3, false);
            }
            break;
        case 4:
            // rook move
            if (b->color[m->end] == 1) {
                if (m->start == 21)
                    change_castling_rights(b, 0, false);
                if (m->start == 28)
                    change_castling_rights(b, 1, false);
            } else {
                if (m->start == 91)
                    change_castling_rights(b, 2, false);
                if (m->start == 98)
                    change_castling_rights(b, 3, false);
            }
            break;
    }

    // change turn and adjust key accordingly
    b->key ^= hashpool[4];
    b->who *= -1;
}
