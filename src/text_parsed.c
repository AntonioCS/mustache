#include "../inc/text_parsed.h"

/**
 * Create object where the parsed text will go
 * 
 * @param m
 * @return 
 */
bool text_parsed_init(pmustache m) {
    if (m->text_size > 0) {
        m->text_parsed = calloc(m->text_size * 2, sizeof (char));

        return (m->text_parsed != NULL);
    }

    return false;
}

void text_parsed_add_char(pmustache m, char *c) {
    m->text_parsed[m->text_parsed_position] = *c;
    m->text_parsed_position++;
}

void text_parsed_add_string(pmustache m, char *s) {
    while (*s != '\0') {
        text_parsed_add_char(m, s);
        s++;
    }
}
