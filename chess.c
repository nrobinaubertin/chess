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

int main(int argc, char* argv[]) {

    srand((unsigned) time(NULL));

    board b = create_board();
    assert(b);
    b = init_board(b);
    print_board(b);

    int color = 1;
    for(int i = 0; i < 100; i++) {
        move m = rand_move(b, color);
        printf("\n");
        print_move(m);
        printf("\n");
        apply_move(m, b);
        print_board(b);
        sleep(4);
        color *= -1;
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

    destroy_board(b);
    return EXIT_SUCCESS;
}
