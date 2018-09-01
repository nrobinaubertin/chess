#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#include "board.h"
#include "print.h"

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

typedef struct move {
    int color;
    int square[28]; /* integers of possible squares */
} *move;

int coord2int(char coord[static 2]) {
    int c = 0;
    switch (coord[0]) {
        case 'a':
            c += 1;
            break;
        case 'b':
            c += 2;
            break;
        case 'c':
            c += 3;
            break;
        case 'd':
            c += 4;
            break;
        case 'e':
            c += 5;
            break;
        case 'f':
            c += 6;
            break;
        case 'g':
            c += 7;
            break;
        case 'h':
            c += 8;
            break;
        default:
            printf("ERROR %s\n", coord);
            exit(1);
    }
    c += 20 + (atoi(&coord[1]) - 1) * 10;
    return c;
}

int main(int argc, char* argv[]) {
    board b = malloc(sizeof(struct board));
    assert(b);
    b = init_board(b);
    print_board(b);

    char coord[3];
    int k;
    while (true) {
        printf("square: ");
        scanf(" %c%c", &coord[0], &coord[1]);
        k = coord2int(coord);
        printf("%d ", k);
        print_square(b->color[k], b->piece[k]);
        printf("\n");
    }

    free(&b);
    return EXIT_SUCCESS;
}
