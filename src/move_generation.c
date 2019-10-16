#include "move_generation.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int ray[5][5] = {
    {21, 19, 12, 8}, // knight
    {11, 9}, // bishop
    {10, 1}, // rook
    {11, 10, 9, 1}, // queen
    {11, 10, 9, 1} // king
};

move_list gen_all_moves(board b) {
    int color = b->who;
    move_list ml = create_move_list();
    add_castle_moves(b, ml, color);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            if (b->color[k] == color) {
                add_move_list(b, k, ml, b->piece[k], b->color[k]);
            }
        }
    }
    return ml;
}

//move rand_move(board b) {
//    int color = b->who;
//    move* ml = create_move_list();
//    int n = 0;
//    for (int i = 0; i < 8; i++) {
//        for (int j = 0; j < 8; j++) {
//            int k = 21 + j + 10*i;
//            if (b->color[k] == color) {
//                add_move_list(b, k, ml, n, b->piece[k], b->color[k]);
//            }
//        }
//    }
//    n = add_castle_moves(b, ml, n, color);
//    move m = malloc(sizeof(struct move));
//    memcpy(m, ml[rand()%n], sizeof(struct move));
//    destroy_move_list(ml);
//    return m;
//}

// create a move and add it to the move_list
void add_move_to_ml(move_list ml, int start, int end) {
    assert(start > 0);
    assert(start < 120);
    assert(end > 0);
    assert(end < 120);
    move m = malloc(sizeof(struct move));
    m->start = start;
    m->end = end;
    push_move_list(ml, m);
}

move_list add_ray_moves(board b, move_list ml, int piece_type, int square, int color) {
    int i = 0;
    while(ray[piece_type][i] != 0) {
        for(int inv = -1; inv <= 1; inv += 2) {
            int k = 1;
            while (
                b->piece[square + inv*ray[piece_type][i] * k] == 7
                || b->color[square + inv*ray[piece_type][i] * k] == color * -1
            ) {
                add_move_to_ml(ml, square, square + inv*ray[piece_type][i] * k);
                // if we eat an enemy piece, end the ray
                if (b->color[square + inv*ray[piece_type][i] * k] == color * -1) {
                    break;
                }
                k++;
            }
        }
        i++;
    }
    return ml;
}

move_list add_point_moves(board b, move_list ml, int piece_type, int square, int color) {
    int i = 0;
    while(ray[piece_type][i] != 0) {
        for(int inv = -1; inv <= 1; inv += 2) {
            if (
                b->piece[square + inv*ray[piece_type][i]] == 7
                || b->color[square + inv*ray[piece_type][i]] == color * -1
            ) {
                add_move_to_ml(ml, square, square + inv*ray[piece_type][i]);
            }
        }
        i++;
    }
    return ml;
}

move_list add_move_list(board b, int square, move_list ml, int piece, int color) {
    switch (piece) {
        case 1: // PAWN
            if (b->piece[square + 10 * color] == 7) {
                add_move_to_ml(ml, square, square + 10 * color);
                if (
                    (square/10 == 3 || square/10 == 8)
                    && b->piece[square + 20 * color] == 7
                ) {
                    add_move_to_ml(ml, square, square + 20 * color);
                }
            }
            if (b->color[square + 10 * color + 1] == color * -1) {
                add_move_to_ml(ml, square, square + 10 * color + 1);
            }
            if (b->color[square + 10 * color - 1] == color * -1) {
                add_move_to_ml(ml, square, square + 10 * color - 1);
            }
            return ml;
        case 2: // KNIGHT
            add_point_moves(b, ml, 0, square, color);
            return ml;
        case 3: // BISHOP
            add_ray_moves(b, ml, 1, square, color);
            return ml;
        case 4: // ROOK
            add_ray_moves(b, ml, 2, square, color);
            return ml;
        case 5: // QUEEN
            add_ray_moves(b, ml, 3, square, color);
            return ml;
        case 6: // KING
            add_point_moves(b, ml, 4, square, color);
            return ml;
        default:
            return ml;
    }
}

move_list add_castle_moves(board b, move_list ml, int color) {
    // no castle possible if king is checked
    if (is_king_checked(b, color)) {
        return ml;
    }

    if (color == 1) {
        if (
            b->castling_rights[1]
            && b->color[22] == 0
            && b->color[23] == 0
            && b->color[24] == 0
            && !is_square_checked(b, 1, 24)
        ) {
            add_move_to_ml(ml, b->king_square[0], 101);
        }
        if (
            b->castling_rights[0]
            && b->color[27] == 0
            && b->color[26] == 0
            && !is_square_checked(b, 1, 26)
        ) {
            add_move_to_ml(ml, b->king_square[0], 100);
        }
    } else {
        if (
            b->castling_rights[3]
            && b->color[92] == 0
            && b->color[93] == 0
            && b->color[94] == 0
            && !is_square_checked(b, -1, 94)
        ) {
            add_move_to_ml(ml, b->king_square[0], 103);
        }
        if (
            b->castling_rights[2]
            && b->color[97] == 0
            && b->color[96] == 0
            && !is_king_checked(b, color)
            && !is_square_checked(b, -1, 96)
        ) {
            add_move_to_ml(ml, b->king_square[0], 102);
        }
    }
    return ml;
}

bool is_checkmate(board b) {
    if (!is_king_checked(b, b->who)) {
        return false;
    }
    bool checkmate = true;
    move_list ml = gen_all_moves(b);
    for (int i = 0; i < ml->size; i++) {
        board bb = copy_board(b);
        apply_move(ml->list[i], bb);
        // rule out moves that keep the king in check
        if (!is_king_checked(bb, bb->who*-1)) {
            checkmate = false;
            break;
        }
        destroy_board(bb);
    }
    return checkmate;
}

bool is_king_checked(board b, int color) {
    if (color == 1)
        return is_square_checked(b, color, b->king_square[0]);
    return is_square_checked(b, color, b->king_square[1]);
}

// this function places a virtual king of the chosen color of the chose square
// and tests if he is in check
bool is_square_checked(board b, int color, int square) {
    move_list ml = NULL;

    // check for pawn attacks
    if (
        b->piece[square + 10 * color + 1] == 1
        && b->color[square + 10 * color + 1] == color * -1
    )
        return true;
    if (
        b->piece[square + 10 * color - 1] == 1
        && b->color[square + 10 * color - 1] == color * -1
    )
        return true;

    // check knight moves
    ml = create_move_list();
    add_move_list(b, square, ml, 2, color);
    for (int i = 0; i < ml->size; i++) {
        if (
            b->color[ml->list[i]->end] == color*-1
            && b->piece[ml->list[i]->end] == 2
        )
            return true;
    }
    destroy_move_list(ml);

    // check bishop and diagonal queen moves
    ml = create_move_list();
    add_move_list(b, square, ml, 5, color);
    for (int i = 0; i < ml->size; i++) {
        if (
            b->color[ml->list[i]->end] == color*-1
            && (b->piece[ml->list[i]->end] == 3 || b->piece[ml->list[i]->end] == 5)
        )
            return true;
    }
    destroy_move_list(ml);

    // check rook and line queen moves
    ml = create_move_list();
    add_move_list(b, square, ml, 4, color);
    for (int i = 0; i < ml->size; i++) {
        if (
            b->color[ml->list[i]->end] == color*-1
            && (b->piece[ml->list[i]->end] == 4 || b->piece[ml->list[i]->end] == 5)
        )
            return true;
    }
    destroy_move_list(ml);

    return false;
}

// an atomic_move is not a legal move but a raw change to the board
// It's used to apply legal moves
void atomic_move(board b, int start, int end) {
    assert(b != NULL);
    assert(start > 0 && start < 120);
    assert(end > 0 && end < 120);
    /*
    // remove moving piece from the key
    b->key ^= hash_piece(b->color[start], b->piece[start], start);
    // remove the eventual taken piece from the key
    if (b->color[end] != 0) {
        b->key ^= hash_piece(b->color[end], b->piece[end], end);
    }
    */
    b->color[end] = b->color[start];
    b->piece[end] = b->piece[start];
    b->color[start] = 0;
    b->piece[start] = 7;
    // pawn promotion to queen
    if (b->piece[end] == 1 && (end/10 == 2 || end/10 == 9))
        b->piece[end] = 5;
    /*
    // add moving piece to the key
    b->key ^= hash_piece(b->color[end], b->piece[end], end);
    */
}

void change_castling_rights(board b, int castle, bool value) {
    b->castling_rights[castle] = value;
    //b->key ^= hashpool[castle];
}

// apply the input move on the given board
// returns informations about the move
// 0: no info
// 1: it was a capture
// 2: it was a castle move
int apply_move(move m, board b) {
    assert(m != NULL);
    assert(b != NULL);

    int is_capture = 0;

    // add current key to the board keys_history
    int i = 0;
    while (b->keys_history[i] != 0)
        i++;
    b->keys_history[i] = b->key;

    // change turn and adjust key accordingly
    //b->key ^= hashpool[4];
    b->who *= -1;

    // is it a castle move ?
    switch (m->end) {
        default:
            break;
        // white small castle
        case 100:
            atomic_move(b, 25, 27);
            atomic_move(b, 28, 26);
            change_castling_rights(b, 0, false);
            change_castling_rights(b, 1, false);
            return 2;
        // white big castle
        case 101:
            atomic_move(b, 25, 23);
            atomic_move(b, 21, 24);
            change_castling_rights(b, 0, false);
            change_castling_rights(b, 1, false);
            return 2;
        // black small castle
        case 102:
            atomic_move(b, 95, 97);
            atomic_move(b, 98, 96);
            change_castling_rights(b, 2, false);
            change_castling_rights(b, 3, false);
            return 2;
        // black big castle
        case 103:
            atomic_move(b, 95, 93);
            atomic_move(b, 91, 94);
            change_castling_rights(b, 2, false);
            change_castling_rights(b, 3, false);
            return 2;
    }

    // special state changes depending on the moving piece
    switch (b->piece[m->start]) {
        case 6:
            // king move
            if (b->color[m->start] == 1) {
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
            if (b->color[m->start] == 1) {
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

    if (b->piece[m->end] != 7)
        is_capture = 1;

    atomic_move(b, m->start, m->end);

    return is_capture;
}
