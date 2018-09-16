#include <stdlib.h>
#include <assert.h>
#include "evaluate.h"

int evaluate(board b, int color) {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            if (b->piece[k] != 7) {
                switch (b->piece[k]) {
                    case 1:
                        score += b->color[k] * 100;
                        break;
                    case 2:
                        score += b->color[k] * 300;
                        break;
                    case 3:
                        score += b->color[k] * 325;
                        break;
                    case 4:
                        score += b->color[k] * 500;
                        break;
                    case 5:
                        score += b->color[k] * 900;
                        break;
                    case 6:
                        score += b->color[k] * 10000;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return score * color;
}
