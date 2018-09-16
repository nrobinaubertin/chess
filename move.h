#pragma once
#include "board.h"

typedef struct move {
    int start;
    int end;
} *move;

move* create_move_list();
void destroy_move_list(move* ml);
move rand_move(board b, int color);
move* gen_move_list(board b, int square);
void apply_move(move m, board b);
void add_move_to_ml(move* ml, int nb_moves, int start, int end);
void add_ray_moves(board b, move* ml, int piece_type, int square);
void add_point_moves(board b, move* ml, int piece_type, int square);
bool is_square_checked(board b, int color, int square);
int add_castle_moves(board b, move* ml, int nb_moves, int color);
