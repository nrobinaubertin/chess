#include "move_list.h"
#include <assert.h>
#include <stdlib.h>

move_list create_move_list() {
    return calloc(1, sizeof(struct move_list));
}

void push_move_list(move_list ml, move m) {
    assert(m);
    assert(ml);
    ml->list = realloc(ml->list, sizeof(move) * (ml->size + 1));
    assert(ml->list);
    for (int i = 0; i < ml->size; i++) {
        assert(ml->list[i]->start > 0 && ml->list[i]->start < 120);
        assert(ml->list[i]->end > 0 && ml->list[i]->end < 120);
    }
    ml->list[ml->size] = m;
    ml->size = ml->size + 1;
}

move pop_move_list(move_list ml) {
    if (ml->size == 0) {
        return NULL;
    }
    move m = ml->list[ml->size - 1];
    ml->list = realloc(ml->list, sizeof(move) * (ml->size - 1));
    ml->size = ml->size - 1;
    return m;
}

void destroy_move_list(move_list ml) {
    ml->list = realloc(ml->list, 0);
    free(ml);
}
