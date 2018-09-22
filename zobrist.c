#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "zobrist.h"

#define HASHTABLE_SIZE 1000000

uint64_t* hashpool;
entry* hashtable;

// rand function to init hash pool
static uint64_t get64rand() {
    return
        (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) |
        (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) |
        (((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
        (((uint64_t) rand() << 48) & 0xFFFF000000000000ull);
}

void init_hashpool() {
    hashpool = malloc(sizeof(uint64_t) * 1450);
    for (int32_t i = 0; i < 1450; i++) {
        hashpool[i] = get64rand();
    }
}

void init_hashtable() {
    hashtable = calloc(HASHTABLE_SIZE, sizeof(entry));
}

entry create_entry(board b, int depth) {
    entry e = malloc(sizeof(struct entry));
    e->key = b->key;
    e->score = b->score;
    e->depth = depth;
    e->next = NULL;
    return e;
}


void destroy_entry(entry e) {
    if (!e)
        return;
    destroy_entry(e->next);
    free(e);
}

void destroy_hashtable() {
    if (!hashtable)
        return;
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        destroy_entry(hashtable[i]);
    }
    free(hashtable);
}

void add_hashtable(entry e) {
    entry fe = hashtable[e->key % HASHTABLE_SIZE];
    if (!fe) {
        hashtable[e->key % HASHTABLE_SIZE] = e;
        return;
    }
    int d = 0;
    entry ee = fe;
    while (ee->next && ee->next->key != e->key) {
        ee = ee->next;
        d++;
    }
    // clean old entries
    if (d > 10) {
        printf("clean\n");
        entry nf = fe->next->next->next;
        fe->next->next->next = NULL;
        destroy_entry(fe);
        hashtable[e->key % HASHTABLE_SIZE] = nf;
    }
    destroy_entry(ee->next);
    ee->next = e;
}

entry find_hashtable(uint64_t key) {
    entry e = hashtable[key % HASHTABLE_SIZE];
    if (!e)
        return NULL;
    if (e->key == key)
        return e;
    while (e->next && e->key != key)
        e = e->next;
    if (e->key == key)
        return e;
    return NULL;
}
