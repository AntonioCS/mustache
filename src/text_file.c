#include "../inc/text_file.h"
#include "../inc/position_file.h"

char text_file_get_char(pmustache m) {
    return fgetc(m->mfile);
}

/**
 * This will get the char at position n but it will also set the position to the location n
 * Reset the position to original position
 * @param m
 * @param n
 * @return
 */
char text_file_get_char_pos(pmustache m, int n) {
    if (n < m->text_size) {
        fseek(m->mfile, n, SEEK_SET);
        return text_file_get_char(m);
    }
    return EOF;
}

void text_file_set_size(pmustache m, long int size) {
    m->text_size = (size_t)size;
}