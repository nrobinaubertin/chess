#include <stdio.h>

#include "print.h"
#include "board.h"

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
