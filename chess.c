#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "util.h"
#include "board.h"
#include "print.h"
#include "move.h"
#include "evaluate.h"

int is_game_over(board b) {
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

int negamax(board b, int color, int depth) {
    if (depth == 0 || is_game_over(b))
        return evaluate(b, color);
    int max = -100000;
    move* ml = gen_all_moves(b, color);
    int i = 0;
    while (ml[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml[i], bb);
        int score = -negamax(bb, color*-1, depth - 1);
        if (score > max)
            max = score;
        destroy_board(bb);
        i++;
    }
    destroy_move_list(ml);
    return max;
}

move best_move(board b, int color, int depth) {
    move best_move = NULL;
    int max = -10000;
    move* ml = gen_all_moves(b, color);
    int i = 0;
    while (ml[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml[i], bb);
        int score = -negamax(bb, color*-1, depth - 1);
        // printf("\n");
        // print_move(ml[i]);
        // printf("score: %d\n", score);
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

int main(int argc, char* argv[]) {

    //srand((unsigned) time(NULL));
    board b = create_board();
    assert(b);
    b = init_board(b);
    print_board(b);

    int color = 1;
    int w = 0;
    while (!(w = is_game_over(b))) {
        move m = best_move(b, color, atoi(argv[1]));
        printf("\n");
        print_move(m);
        printf("\n");
        apply_move(m, b);
        print_board(b);
        free(m);
        color *= -1;
    }
    destroy_board(b);

    if (w == 1) {
        printf("White wins !\n");
    } else {
        printf("Black wins !\n");
    }

    // char coord[3];
    // int k;
    // while (true) {
    //     printf("square: ");
    //     scanf(" %c%c", &coord[0], &coord[1]);
    //     k = coord2int(coord);
    //     printf("%d ", k);
    //     print_square(b->color[k], b->piece[k]);
    //     printf("\n");
    // }

    return EXIT_SUCCESS;
}
