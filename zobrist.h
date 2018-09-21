#pragma once
#include <stdint.h>
#include "board.h"

typedef struct entry {
    board b;
    struct entry* next;
} *entry;
extern uint64_t* hashpool;
extern entry* hashtable;
void init_hashpool();
void init_hashtable();
void add_hashtable(board b);
board find_hashtable(uint64_t key);
void destroy_hashtable();
void destroy_entry(entry e);
