#include "../inc/position.h"

size_t pos_get(pmustache m) {
    return m->text_position;
}

void pos_inc(pmustache m) {
    m->text_position++;
}

void pos_dec(pmustache m) {
    m->text_position--;
}

size_t pos_get_inc(pmustache m) {
    int x = pos_get(m);
    pos_inc(m);
    return x;
}

void pos_set(pmustache m, int p) {
    m->text_position = p;
}
