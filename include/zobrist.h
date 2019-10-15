#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "board.h"
#include <stdint.h>

typedef struct entry {
    uint64_t key;
    int score;
    int depth;
    struct entry* next;
} *entry;

extern uint64_t* hashpool;
extern entry* hashtable;
void init_hashpool();
void init_hashtable();
void add_hashtable(entry e);
entry find_hashtable(uint64_t key);
void destroy_hashtable();
void destroy_entry(entry e);
entry create_entry(board b, int depth);

#endif
