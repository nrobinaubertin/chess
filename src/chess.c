#include "evaluate.h"
#include "move_generation.h"
#include "print.h"
#include "board.h"
#include "move.h"
#include "move_list.h"
#include "util.h"
//#include "zobrist.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int is_game_over(board b, bool check_draws) {
    // check for draw
    //if (check_draws) {
    //    int n = 0;
    //    int it = 0;
    //    while (b->keys_history[it] != 0) {
    //        it++;
    //        if (b->key == b->keys_history[it])
    //            n++;
    //    }
    //    if (n > 2)
    //        return 100;
    //}
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

int search(board b, int depth, int alpha, int beta) {
    //entry e = find_hashtable(b->key);
    //if (e && e->depth > depth)
    //    return e->score;
    if (depth == 0)
        return evaluate(b);
    int state = is_game_over(b, true);
    if (state) {
        if (state == 100)
            return 0;
        return evaluate(b);
    }
    int value = -100000;
    move_list ml = gen_all_moves(b);
    int i = 0;
    while (ml->list[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml->list[i], bb);
        bb->score = -search(bb, depth - 1, beta*-1, alpha*-1);
        value = max(value, bb->score);
        alpha = max(alpha, value);
        //if (depth > 2)
        //    add_hashtable(create_entry(bb, depth));
        destroy_board(bb);
        if (alpha >= beta)
            break;
        i++;
    }
    destroy_move_list(ml);
    return value;
}

int MTDF(board b, int f, int depth) {
    int upper = 100000;
    int lower = -100000;
    while (lower < upper) {
        int beta = max(f, lower + 1);
        f = search(b, depth, beta - 1, beta);
        if (f < beta) {
            upper = f;
        } else {
            lower = f;
        }
    }
    return f;
}

move best_move(board b, int depth, bool display) {
    move best_move = NULL;
    int max = -10000;
    move_list ml = gen_all_moves(b);
    int i = 0;
    while (ml->list[i] != NULL) {
        board bb = copy_board(b);
        apply_move(ml->list[i], bb);
        int score = -MTDF(bb, 0, depth - 1);
        if (display) {
            printf("\n");
            print_move(ml->list[i]);
            printf("score: %d\n", score);
        }
        if (score > max) {
            max = score;
            best_move = ml->list[i];
        }
        destroy_board(bb);
        i++;
    }
    best_move = copy_move(best_move);
    destroy_move_list(ml);
    return best_move;
}

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
    move_list ml = gen_all_moves(b);
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
void play_alone(int depth) {
    srand((unsigned) time(NULL));
    //init_hashtable();
    //init_hashpool();
    board b = create_board();
    init_board(b);
    print_board(b);

    int w = 0;
    int turn = 0;
    while (!(w = is_game_over(b, true))) {
        move m = best_move(b, depth, false);
        printf("\n");
        print_move(m);
        printf("\n");
        apply_move(m, b);
        print_board(b);
        if (m)
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
    //free(hashpool);
    //destroy_hashtable();
}

void find_best_starting_move(int depth) {
    srand((unsigned) time(NULL));
    //init_hashtable();
    //init_hashpool();
    board b = create_board();
    init_board(b);
    print_board(b);
    move m = best_move(b, depth, true);
    printf("\n");
    print_move(m);
    printf("\n");
    apply_move(m, b);
    print_board(b);
    free(m);
    destroy_board(b);
}

// execute perft test
void execute_perft(int depth) {
    //srand((unsigned) time(NULL));
    //init_hashtable();
    //init_hashpool();
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
    // TODO handle castling
    move m = malloc(sizeof(struct move));
    m->start = coord2int(start);
    m->end = coord2int(end);
    return m;
}

// play against human
void play(int color, int depth) {
    srand((unsigned) time(NULL));
    //init_hashtable();
    //init_hashpool();
    board b = create_board();
    init_board(b);
    print_board(b);

    int w = 0;
    move m = NULL;
    while (!(w = is_game_over(b, true))) {
        if (b->who == color) {
            m = best_move(b, depth, false);
        } else {
            m = askForMove();
        }
        printf("\n");
        print_move(m);
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
    //free(hashpool);
    //destroy_hashtable();
}

int main(int argc, char* argv[]) {
    if (!argv[1]) {
        printf("No argument !\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "start") == 0) {
        find_best_starting_move(atoi(argv[2]));
    } else if(strcmp(argv[1], "perft") == 0) {
        if (!argv[2]) {
            printf("No perft depth !\n");
            return EXIT_FAILURE;
        }
        execute_perft(atoi(argv[2]));
    } else if(strcmp(argv[1], "play") == 0) {
        play(-1, atoi(argv[2]));
    } else if(strcmp(argv[1], "alone") == 0) {
        play_alone(atoi(argv[2]));
    } else {
        printf("Unknown command.\n");
    }
    return EXIT_SUCCESS;
}
