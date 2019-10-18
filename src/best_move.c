#include "evaluate.h"
#include "print.h"
#include "board.h"
#include "move.h"
#include "move_list.h"
#include "move_generation.h"
#include "util.h"
#include <pthread.h>
#include <math.h>
#include <stdio.h>

int is_game_over(board b, bool check_draws) {
    // check for draw
    //if (check_draws) {
    //    int n = 0;
    //    int it = 0;
    //    while (b->keys_history[it] != 0) {
    //        it++;
    //        if (b->key == b->keys_history[it])
    //            n++;
    //    }
    //    if (n > 2)
    //        return 100;
    //}
    bool white = false;
    bool black = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            if (b->piece[k] == 6) {
                if (b->color[k] == 1) {
                    white = true;
                } else {
                    black = true;
                }
            }
            if (white && black) {
                return 0;
            }
        }
    }
    if (white)
        return 1;
    return -1;
}

int search(board b, int depth, int alpha, int beta) {
    //entry e = find_hashtable(b->key);
    //if (e && e->depth > depth)
    //    return e->score;
    if (depth == 0)
        return evaluate(b);
    int state = is_game_over(b, true);
    if (state) {
        if (state == 100)
            return 0;
        return evaluate(b);
    }
    int value = -100000;
    move_list ml = gen_all_moves(b);
    for (int i = 0; i < ml->size; i++) {
        board bb = copy_board(b);
        apply_move(ml->list[i], bb);
        bb->score = -search(bb, depth - 1, beta*-1, alpha*-1);
        value = max(value, bb->score);
        alpha = max(alpha, value);
        //if (depth > 2)
        //    add_hashtable(create_entry(bb, depth));
        destroy_board(bb);
        if (alpha >= beta)
            break;
    }
    destroy_move_list(ml);
    return value;
}

int MTDF(board b, int f, int depth) {
    int upper = 100000;
    int lower = -100000;
    while (lower < upper) {
        int beta = max(f, lower + 1);
        f = search(b, depth, beta - 1, beta);
        if (f < beta) {
            upper = f;
        } else {
            lower = f;
        }
    }
    return f;
}

move best_move(board b, int depth, bool display) {
    move best_move = NULL;
    int max = -10000;
    move_list ml = gen_all_moves(b);
    for (int i = 0; i < ml->size; i++) {
        board bb = copy_board(b);
        apply_move(ml->list[i], bb);
        int score = -MTDF(bb, 0, depth - 1);
        if (display) {
            printf("\n");
            print_move(ml->list[i], true);
            printf("score: %d\n", score);
        }
        if (score > max) {
            max = score;
            best_move = ml->list[i];
        }
        destroy_board(bb);
    }
    best_move = copy_move(best_move);
    destroy_move_list(ml);
    return best_move;
}
