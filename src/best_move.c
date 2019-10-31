#include "best_move.h"
#include "zobrist.h"
#include "multithread.h"
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
#include <stdlib.h>

extern int collisions;
extern int znodes;

arg_t create_arg_t(board b, int f, int depth, int qdepth, z_hashtable z_ht) {
    arg_t input = calloc(1, sizeof(struct arg_t));
    input->b = b;
    input->f = f;
    input->depth = depth;
    input->qdepth = qdepth;
    input->z_ht = z_ht;
    return input;
}

// returns:
// 1: white wins
// -1: black wins
// 0: game is ongoing
// 100: it's a draw
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

// Used by the MTDF search (not directly)
// alpha-beta pruning
int search(board b, int depth, int alpha, int beta, int qdepth, z_hashtable z_ht) {
    if (depth == 0) {
        return evaluate(b);
    }
    int state = is_game_over(b, true);
    if (state) {
        if (state == 100)
            return 0;
        return state * 10000;
        //return evaluate(b);
    }

    // search node in hashtable
    uint64_t key = 0;
    if (z_ht != NULL) {
        key = hash_board(b, z_ht->hashpool);
        entry z_node = find_hashtable(z_ht, key);
        if (z_node != NULL && z_node->payload != NULL) {
            if (!cmp_board(z_node->payload, b)) {
                collisions++;
            }
            if (z_node->depth > depth) {
                return z_node->value;
            }
        }
    }

    // are we in quiescent search ?
    bool quiescent = depth < qdepth;

    int value = -10000;
    move_list ml = gen_all_moves(b, quiescent);
    if (ml->size == 0 && quiescent) {
        destroy_move_list(ml);
        return evaluate(b);
    }
    for (int i = 0; i < ml->size; i++) {
        board bb = copy_board(b);
        apply_move(ml->list[i], bb);
        bb->score = -search(bb, depth - 1, beta*-1, alpha*-1, qdepth, z_ht);
        value = max(value, bb->score);
        alpha = max(alpha, value);
        destroy_board(bb);
        if (alpha >= beta)
            break;
    }
    destroy_move_list(ml);

    if (z_ht != NULL) {
        entry e = create_entry();
        e->key = key;
        e->value = value;
        e->depth = depth;
        e->payload = copy_board(b);
        add_hashtable(z_ht, e);
        znodes++;
    }
    return value;
}

// MTDF search: https://en.wikipedia.org/wiki/MTD-f
void* MTDF(void* input) {
    board b = ((arg_t)input)->b;
    int f = evaluate(b);//((arg_t)input)->f;
    int depth = ((arg_t)input)->depth;
    int qdepth = ((arg_t)input)->qdepth;
    z_hashtable z_ht = ((arg_t)input)->z_ht;
    int upper = 10000;
    int lower = -10000;

    while (lower < upper) {
        int beta = max(f, lower + 1);
        f = search(b, depth, beta - 1, beta, qdepth, z_ht);
        if (f < beta) {
            upper = f;
        } else {
            lower = f;
        }
    }
    int* ret = calloc(1, sizeof(int));
    *ret = f;
    return (void*) ret;
}

// can return NULL, if it's the case, it's a resignation
move best_move(board b, int depth, int threads, int qdepth, z_hashtable z_ht) {
    move best_move = NULL;
    move_list ml = gen_all_moves(b, false);
    task_list tl = create_task_list();
    for (int i = 0; i < ml->size; i++) {
        board bb = copy_board(b);
        apply_move(ml->list[i], bb);
        arg_t input = create_arg_t(bb, 0, depth - 1, qdepth, z_ht);
        push_task_list(tl, create_task((void*) input, MTDF));
    }

    do_task_list(tl, threads);

    int max = -10000;
    for (int i = 0; i < tl->size; i++) {
        assert(tl->list[i]->result);
        int score = -*((int *)tl->list[i]->result);
        if (score > max) {
            max = score;
            best_move = ml->list[i];
        }
        destroy_board(((arg_t)tl->list[i]->payload)->b);
        free(tl->list[i]->payload);
        free(tl->list[i]->result);
    }
    best_move = copy_move(best_move);
    destroy_task_list(tl);
    destroy_move_list(ml);
    return best_move;
}
