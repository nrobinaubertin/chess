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

int main(int argc, char* argv[]) {

    int max_t = 0;
    int seed = 0;

    for(int s = 300; s < 900; s++) {

        srand(s);
        // srand((unsigned) time(NULL));

        board b = create_board();
        assert(b);
        b = init_board(b);
        //print_board(b);

        int color = 1;
        int w = 0;
        int turn = 0;
        while(! (w = is_game_over(b))) {
            move m = rand_move(b, color);
            // printf("\n");
            // print_move(m);
            // printf("\n");
            apply_move(m, b);
            // print_board(b);
            // sleep(2);
            color *= -1;
            turn++;
            free(m);
        }

        if (turn > max_t) {
            max_t = turn;
            seed = s;
        }
        destroy_board(b);
    // if (w == 1) {
    //     printf("White wins !\n");
    // } else {
    //     printf("Black wins !\n");
    // }
    // printf("The play lasted %d turns.\n", turn);
    }
    printf("%d %d\n", seed, max_t);

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
