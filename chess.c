#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "evaluate.h"
#include "move.h"
#include "print.h"
#include "util.h"
#include "zobrist.h"

int is_game_over(board b, bool check_draws) {
    // check for draw
    if (check_draws) {
        int n = 0;
        int it = 0;
        while (b->keys_history[it] != 0) {
            it++;
            if (b->key == b->keys_history[it])
                n++;
        }
        if (n > 2)
            return 100;
    }
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
    entry e = find_hashtable(b->key);
    if (e && e->depth > depth)
        return e->score;
    if (depth == 0)
        return evaluate(b);
    int state = is_game_over(b, true);
    if (state) {
        if (state == 100)
            return 0;
        return evaluate(b);
    }
    int value = -100000;
    move* ml = gen_all_moves(b);
    int i = 0;
    while (ml[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml[i], bb);
        bb->score = -search(bb, depth - 1, beta*-1, alpha*-1);
        value = max(value, bb->score);
        alpha = max(alpha, value);
        if (depth > 2)
            add_hashtable(create_entry(bb, depth));
        destroy_board(bb);
        if (alpha >= beta)
            break;
        i++;
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
    move* ml = gen_all_moves(b);
    int i = 0;
    while (ml[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml[i], bb);
        int score = -MTDF(bb, 0, depth - 1);
        if (display) {
            printf("\n");
            print_move(ml[i]);
            printf("score: %d\n", score);
        }
        if (score > max) {
            max = score;
            best_move = ml[i];
        }
        destroy_board(bb);
        i++;
    }
    best_move = copy_move(best_move);
    destroy_move_list(ml);
    return best_move;
}

int checks = 0;
int perft(board b, int depth) {
    if (depth == 0) {
        if (is_king_checked(b, b->who))
            checks++;
        return 1;
    }
    int nodes = 0;
    move* ml = gen_all_moves(b);
    int i = 0;
    while (ml[i]) {
        board bb = copy_board(b);
        apply_move(ml[i], bb);
        if (!is_king_checked(bb, bb->who*-1))
            nodes += perft(bb, depth - 1);
        destroy_board(bb);
        i++;
    }
    destroy_move_list(ml);
    return nodes;
}

void play_alone(int depth) {
    srand((unsigned) time(NULL));
    init_hashtable();
    init_hashpool();
    board b = create_board();
    init_board(b);
    print_board(b);

    int w = 0;
    int turn = 0;
    while (!(w = is_game_over(b, true))) {
        move m = best_move(b, depth, false);
        printf("\n");
        print_move(m);
        printf("\n");
        apply_move(m, b);
        print_board(b);
        if (m)
            free(m);
        turn++;
    }

    if (w == 1) {
        printf("White wins !\n");
    } else if (w == -1) {
        printf("Black wins !\n");
    } else {
        printf("Draw !\n");
    }
    free(hashpool);
    destroy_hashtable();
}

void find_best_starting_move(int depth) {
    srand((unsigned) time(NULL));
    init_hashtable();
    init_hashpool();
    board b = create_board();
    init_board(b);
    print_board(b);
    move m = best_move(b, depth, true);
    printf("\n");
    print_move(m);
    printf("\n");
    apply_move(m, b);
    print_board(b);
    free(m);
    destroy_board(b);
}

void execute_perft(int depth) {
    srand((unsigned) time(NULL));
    init_hashtable();
    init_hashpool();
    board b = create_board();
    init_board(b);
    printf("nodes: %d\n", perft(b, depth));
    printf("checks: %d\n", checks);
    destroy_board(b);
}

move askForMove() {
    char start[2];
    char end[2];
    printf("move: ");
    scanf(" %c%c%c%c", &start[0], &start[1], &end[0], &end[1]);
    // TODO handle castling
    move m = malloc(sizeof(struct move));
    m->start = coord2int(start);
    m->end = coord2int(end);
    return m;
}

void play(int color, int depth) {
    srand((unsigned) time(NULL));
    init_hashtable();
    init_hashpool();
    board b = create_board();
    init_board(b);
    print_board(b);

    int w = 0;
    move m = NULL;
    while (!(w = is_game_over(b, true))) {
        if (b->who == color) {
            m = best_move(b, depth, false);
        } else {
            m = askForMove();
        }
        printf("\n");
        print_move(m);
        printf("\n");
        apply_move(m, b);
        print_board(b);
        free(m);
    }

    if (w == 1) {
        printf("White wins !\n");
    } else if (w == -1) {
        printf("Black wins !\n");
    } else {
        printf("Draw !\n");
    }
    free(hashpool);
    destroy_hashtable();
}

int main(int argc, char* argv[]) {
    // find_best_starting_move(atoi(argv[1]));
    // execute_perft(atoi(argv[1]));
    play(-1, atoi(argv[1]));
    return EXIT_SUCCESS;
}
