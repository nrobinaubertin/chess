#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#include "util.h"
#include "board.h"
#include "print.h"
#include "move.h"
#include "evaluate.h"
#include "zobrist.h"

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

int search(board b, int depth) {
    entry e = find_hashtable(b->key);
    if (e)
        return e->score;
    if (depth == 0 || is_game_over(b))
        return evaluate(b);
    int max = -100000;
    move* ml = gen_all_moves(b);
    int i = 0;
    while (ml[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml[i], bb);
        bb->score = -search(bb, depth - 1);
        if (bb->score > max)
            max = bb->score;
        add_hashtable(create_entry(bb, depth));
        destroy_board(bb);
        i++;
    }
    destroy_move_list(ml);
    return max;
}

move best_move(board b, int depth) {
    move best_move = NULL;
    int max = -10000;
    move* ml = gen_all_moves(b);
    int i = 0;
    while (ml[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml[i], bb);
        int score = -search(bb, depth - 1);
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
    srand((unsigned) time(NULL));
    init_hashtable();
    init_hashpool();
    board b = create_board();
    b = init_board(b);
    print_board(b);

    move m = best_move(b, atoi(argv[1]));
    printf("\n");
    print_move(m);
    printf("\n");
    apply_move(m, b);
    print_board(b);
    free(m);
    destroy_board(b);
    // while (!(w = is_game_over(b))) {
    //     //move m = best_move(b, atoi(argv[1]));
    //     move m = rand_move(b);
    //     //printf("\n");
    //     //print_move(m);
    //     //printf("\n");
    //     apply_move(m, b);
    //     //print_board(b);
    //     //sleep(2);
    //     if (m)
    //         free(m);
    // }

    // if (w == 1) {
    //     printf("White wins !\n");
    // } else {
    //     printf("Black wins !\n");
    // }

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

    free(hashpool);
    destroy_hashtable();
    return EXIT_SUCCESS;
}
