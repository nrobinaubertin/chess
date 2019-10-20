#include "evaluate.h"
#include "move_generation.h"
#include "print.h"
#include "best_move.h"
#include "board.h"
#include "move.h"
#include "move_list.h"
#include "util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// perft test: recursively count all move nodes and checks
// compare results with https://chessprogramming.org/Perft_Results
// obviously, since this engine doesn't take "en passant" into account,
// values are going to differ starting depth 5
int checks = 0;
int captures = 0;
int castles = 0;
int checkmates = 0;
int perft(board b, int depth, int last_move_type) {

    // count various statistics for leaf nodes
    if (depth == 0) {
        if (is_king_checked(b, b->who)) {
            checks++;
        }
        if (is_checkmate(b)) {
            checkmates++;
        }
        switch (last_move_type) {
            case 2:
                castles++;
                break;
            case 1:
                captures++;
                break;
            default:
                break;
        }
        return 1;
    }

    int nodes = 0;
    move_list ml = gen_all_moves(b, false);
    for (int i = 0; i < ml->size; i++) {
        board bb = copy_board(b);
        last_move_type = apply_move(ml->list[i], bb);
        // rule out moves that keep the king in check
        if (!is_king_checked(bb, bb->who*-1)) {
            nodes += perft(bb, depth - 1, last_move_type);
        }
        destroy_board(bb);
    }
    destroy_move_list(ml);
    return nodes;
}

// this will make the AI play against itself
void play_alone(int depth, int duration, bool debug, int threads, int qdepth) {
    board b = create_board();
    init_board(b);
    if (debug) {
        print_board(b);
        printf("\n");
    }

    int w = 0;
    int turn = 0;
    while (!(w = is_game_over(b, true)) && turn < duration) {
        move m = best_move(b, depth, threads, qdepth);

        if (!m) {
            if (b->who == 1) {
                printf("White resigns !\n");
            } else {
                printf("Black resigns !\n");
            }
            printf("\n");
            print_board(b);
            printf("\n");
            destroy_board(b);
            return;
        }

        print_move(m, debug);
        apply_move(m, b);
        if (debug) {
            printf("\n");
            print_board(b);
            printf("\n");
        }
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
    destroy_board(b);
}

// execute perft test
void execute_perft(int depth) {
    board b = create_board();
    init_board(b);
    printf("nodes: %d\n", perft(b, depth, 0));
    printf("checks: %d\n", checks);
    printf("castles: %d\n", castles);
    printf("captures: %d\n", captures);
    printf("checkmates: %d\n", checkmates);
    destroy_board(b);
}

// prompt human for next move
move askForMove() {
    char start[2];
    char end[2];
    printf("move: ");
    scanf(" %c%c%c%c", &start[0], &start[1], &end[0], &end[1]);
    move m = create_move();
    m->start = coord2int(start);
    m->end = coord2int(end);
    return m;
}

// play against human
void play(int color, int depth, int threads, int qdepth) {
    board b = create_board();
    init_board(b);
    print_board(b);

    int w = 0;
    move m = NULL;
    while (!(w = is_game_over(b, true))) {
        if (b->who == color) {
            m = best_move(b, depth, threads, qdepth);
            if (!m) {
                if (b->who == 1) {
                    printf("White resigns !\n");
                } else {
                    printf("Black resigns !\n");
                }
                printf("\n");
                print_board(b);
                printf("\n");
                destroy_board(b);
                return;
            }
        } else {
            m = askForMove();
        }
        printf("\n");
        print_move(m, false);
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
}

int main(int argc, char* argv[]) {
    if (!argv[1]) {
        printf("No argument !\n");
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "perft") == 0) {
        if (!argv[2]) {
            printf("No depth !\n");
            return EXIT_FAILURE;
        }
        execute_perft(atoi(argv[2]));
    } else if(strcmp(argv[1], "play") == 0) {
        if (argc < 3) {
            printf("chess play <depth> <threads>\n");
            return EXIT_FAILURE;
        }
        int depth = atoi(argv[2]);
        int threads = atoi(argv[3]);
        play(-1, depth, threads, depth);
    } else if(strcmp(argv[1], "alone") == 0) {
        if (argc < 4) {
            printf("chess alone <depth> <duration> <threads>\n");
            return EXIT_FAILURE;
        }
        int depth = atoi(argv[2]);
        int duration = atoi(argv[3]);
        int threads = atoi(argv[4]);
        play_alone(depth, duration, false, threads, depth);
    } else {
        printf("Unknown command.\n");
    }
    return EXIT_SUCCESS;
}
