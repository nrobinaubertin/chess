#include <stdlib.h>

#include "evaluate.h"

// Piece-Square Tables

const int eval_pawn[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
    0,  5, 10, 10,-20,-20, 10, 10,  5, 0,
    0,  5, -5,-10,  0,  0,-10, -5,  5, 0,
    0,  0,  0,  0, 20, 20,  0,  0,  0, 0,
    0,  5,  5, 10, 25, 25, 10,  5,  5, 0,
    0, 10, 10, 20, 30, 30, 20, 10, 10, 0,
    0, 50, 50, 50, 50, 50, 50, 50, 50, 0,
    0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_knight[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,-50,-40,-30,-30,-30,-30,-40,-50,0,
    0,-40,-20,  0,  5,  5,  0,-20,-40,0,
    0,-30,  5, 10, 15, 15, 10,  5,-30,0,
    0,-30,  0, 15, 20, 20, 15,  0,-30,0,
    0,-30,  5, 15, 20, 20, 15,  5,-30,0,
    0,-30,  0, 10, 15, 15, 10,  0,-30,0,
    0,-40,-20,  0,  0,  0,  0,-20,-40,0,
    0,-50,-40,-30,-30,-30,-30,-40,-50,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_bishop[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,-20,-10,-10,-10,-10,-10,-10,-20,0,
    0,-10,  5,  0,  0,  0,  0,  5,-10,0,
    0,-10, 10, 10, 10, 10, 10, 10,-10,0,
    0,-10,  0, 10, 10, 10, 10,  0,-10,0,
    0,-10,  5,  5, 10, 10,  5,  5,-10,0,
    0,-10,  0,  5, 10, 10,  5,  0,-10,0,
    0,-10,  0,  0,  0,  0,  0,  0,-10,0,
    0,-20,-10,-10,-10,-10,-10,-10,-20,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_rook[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
    0,  5, 10, 10, 10, 10, 10, 10,  5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0,  0,  0,  0,  5,  5,  0,  0,  0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_queen[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,-20,-10,-10, -5, -5,-10,-10,-20,0,
    0,-10,  0,  5,  0,  0,  0,  0,-10,0,
    0,-10,  5,  5,  5,  5,  5,  0,-10,0,
    0,  0,  0,  5,  5,  5,  5,  0, -5,0,
    0, -5,  0,  5,  5,  5,  5,  0, -5,0,
    0,-10,  0,  5,  5,  5,  5,  0,-10,0,
    0,-10,  0,  0,  0,  0,  0,  0,-10,0,
    0,-20,-10,-10, -5, -5,-10,-10,-20,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int evaluate(board b) {
    int score = 0;
    int color = b->who;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i;
            switch (b->piece[k]) {
                case 1:
                    // doubled or blocked pawns
                    if (b->piece[k + 10 * color] == 1) {
                        score += b->color[k] * 40;
                    } else {
                        score += b->color[k] * 100;
                    }
                    if (b->color[k] == 1) {
                        score += eval_pawn[k];
                    } else {
                        int kk = 21 + j + 70 - 10*i; // inverted k (for black piece square tables)
                        score += eval_pawn[kk];
                    }
                    break;
                case 2:
                    if (b->color[k] == 1) {
                        score += eval_knight[k] + 300;
                    } else {
                        int kk = 21 + j + 70 - 10*i; // inverted k (for black piece square tables)
                        score += eval_knight[kk] - 300;
                    }
                    break;
                case 3:
                    if (b->color[k] == 1) {
                        score += eval_bishop[k] + 325;
                    } else {
                        int kk = 21 + j + 70 - 10*i; // inverted k (for black piece square tables)
                        score += eval_bishop[kk] - 325;
                    }
                    break;
                case 4:
                    if (b->color[k] == 1) {
                        score += eval_rook[k] + 500;
                    } else {
                        int kk = 21 + j + 70 - 10*i; // inverted k (for black piece square tables)
                        score += eval_rook[kk] - 500;
                    }
                    break;
                case 5:
                    if (b->color[k] == 1) {
                        score += eval_queen[k] + 900;
                    } else {
                        int kk = 21 + j + 70 - 10*i; // inverted k (for black piece square tables)
                        score += eval_queen[kk] - 900;
                    }
                    break;
                case 6:
                    //this should be checked at game state in search()
                    //score += b->color[k] * 10000;
                    //break;
                default:
                    break;
            }
        }
    }
    return score * color;
}
