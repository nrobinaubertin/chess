#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

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

typedef struct board {
    int color[120]; /* LIGHT = 1, DARK = -1, or EMPTY = 0 */
    int piece[120]; /* NOT USED, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY */
} *board;

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

void print_square(int color, int piece) {
    if (color == 1) {
        switch (piece) {
            case 0:
                printf("x");
                break;
            case 1:
                printf("\u2659");
                break;
            case 2:
                printf("\u2658");
                break;
            case 3:
                printf("\u2657");
                break;
            case 4:
                printf("\u2656");
                break;
            case 5:
                printf("\u2655");
                break;
            case 6:
                printf("\u2654");
                break;
            case 7:
                printf(".");
                break;
        }
    } else {
        switch (piece) {
            case 0:
                printf("x");
                break;
            case 1:
                printf("\u265F");
                break;
            case 2:
                printf("\u265E");
                break;
            case 3:
                printf("\u265D");
                break;
            case 4:
                printf("\u265C");
                break;
            case 5:
                printf("\u265B");
                break;
            case 6:
                printf("\u265A");
                break;
            case 7:
                printf(".");
                break;
        }
    }
}

void print_board(board b) {
    char letter[8] = "abcdefgh";
    for (int i = 7; i >= 0; i--) {
        printf("%d ", (i + 1));
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            print_square(b->color[k], b->piece[k]);
            printf(" ");
        }
        printf("\n");
    }
    printf("  ");
    for (int i = 0; i < 8; i++) {
        printf("%c ", letter[i]);
    }
    printf("\n");
}

board init_board(board b) {
    struct board static start_board = {
        .color = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, -1, -1, -1, -1, -1, -1, -1, -1, 0,
            0, -1, -1, -1, -1, -1, -1, -1, -1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        .piece = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 4, 2, 3, 5, 6, 3, 2, 4, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
            0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
            0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
            0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 4, 2, 3, 5, 6, 3, 2, 4, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    };
    b = &start_board;
    return b;
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
