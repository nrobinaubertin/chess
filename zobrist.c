#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "zobrist.h"

#define HASHTABLE_SIZE 100000

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
        // printf("%ld\n", hashpool[i]);
    }
}

void init_hashtable() {
    hashtable = calloc(HASHTABLE_SIZE, sizeof(entry));
}

void destroy_entry(entry e) {
    if (!e)
        return;
    if (e->next)
        destroy_entry(e->next);
    free(e->b);
    free(e);
}

void destroy_hashtable() {
    if (!hashtable)
        return;
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        if (hashtable[i])
            destroy_entry(hashtable[i]);
    }
    free(hashtable);
}

void add_hashtable(board b) {
    entry ne = calloc(1, sizeof(struct entry));
    ne->b = b;
    entry e = hashtable[b->key % HASHTABLE_SIZE];
    if (!e) {
        hashtable[b->key % HASHTABLE_SIZE] = ne;
        return;
    }
    while (e->next)
        e = e->next;
    e->next = ne;
}

board find_hashtable(uint64_t key) {
    entry e = hashtable[key % HASHTABLE_SIZE];
    if (!e)
        return NULL;
    if (e->b->key == key)
        return e->b;
    while (e->next && e->b->key != key)
        e = e->next;
    if (e->b->key == key)
        return e->b;
    return NULL;
}
