#pragma once
#include "board.h"

typedef struct move {
    int start;
    int end;
} *move;

move* create_move_list();
void destroy_move_list(move* ml);
move rand_move(board b);
void apply_move(move m, board b);
void add_move_to_ml(move* ml, int n, int start, int end);
int add_ray_moves(board b, move* ml, int n, int piece_type, int square, int color);
int add_point_moves(board b, move* ml, int n, int piece_type, int square, int color);
bool is_square_checked(board b, int color, int square);
int add_castle_moves(board b, move* ml, int nb_moves, int color);
move copy_move(move m);
move* gen_all_moves(board b);
int add_move_list(board b, int square, move* ml, int n);
