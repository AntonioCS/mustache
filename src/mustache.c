#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mustache.h"


static int pos_get(pmustache);
static void pos_inc(pmustache);
static void pos_dec(pmustache);
static int pos_get_inc(pmustache);

static void text_set(pmustache, char *);
static char text_get_char(pmustache);
static char text_get_char_pos(pmustache, int);

static void text_set_size(pmustache);
static int text_get_size(pmustache);

static bool tag_start(pmustache, char *);
static bool tag_start_last(pmustache, char *);
static bool tag_end(pmustache, char *);
static bool tag_end_last(pmustache, char *);
//All of the k* functions will call this one
static bool tag_char(char *, pmustache, char *);
static void tag_read_to_end(pmustache m);
static bool is_tag(pmustache, char *);

static bool text_parsed_init(pmustache);
static void text_parsed_add_char(pmustache, char *);
static void text_parsed_add_string(pmustache m, char *);

static char *text_escape(char *);

pmustache mustache_init() {
    pmustache m = malloc(sizeof (mustache));

    m->text = NULL;
    m->text_parsed = NULL;

    m->tags_values_index = 0;
    m->tags_values_size = (MUSTACHE_TAGS_TOTAL * 2);
    m->tags_values = malloc(sizeof (char *) * m->tags_values_size);
    for (int n = 0;n < m->tags_values_size;n++) {
        m->tags_values[n] = malloc(sizeof (char*));
    }

    m->start_first_char = '{';
    m->start_last_char = '{';
    m->end_first_char = '}';
    m->end_last_char = '}';

    m->text_parsed_position = 0;
    m->text_size = 0;

    m->text_parsed_position = 0;
    m->text_parsed_size = 0;

    return m;
}

void mustache_render(pmustache m) {
    char c;
    bool escape_values = true;

    if (text_parsed_init(m) != true) {
        printf("Unable to initialize text_parsed\n");
        exit(-1);
    }

    while ((c = text_get_char(m)) != EOF) {

        //check if this is the start of a tag
        if (tag_start(m, &c)) {
            char tag_char = text_get_char(m);

            if (tag_start_last(m, &tag_char)) {
                char tag_type = text_get_char(m);

                switch (tag_type) {

                        //do not escape characters
                    case '{': //will have an extra } as close char
                        escape_values = false;
                        break;
                        //Sections
                    case '#':
                        break;
                        //Inverted Sections
                    case '^':
                        break;

                        //Comments
                    case '!':
                        break;

                        //Partials (I believe this is basically a way to include other templates)
                    case '>':
                        break;

                        //Set Delimiter
                    case '=':
                        break;



                    default:
                        ;
                }


                //Default value tag
                char *ptag = calloc(MUSTACHE_TAGS_MAX_LEN_SIZE, sizeof (char));
                int tag_index = 0;


                while ((tag_char = text_get_char(m))) {

                    //if (escape_values == false)

                    if (tag_end(m, &tag_char)) {
                        char tmp_char = text_get_char(m);
                        if (tag_end_last(m, &tmp_char)) {
                            break;
                        }
                        else {
                            pos_dec(m);
                        }
                    }

                    ptag[tag_index] = tag_char;
                    tag_index++;

                    if (tag_index > MUSTACHE_TAGS_MAX_LEN_SIZE) {
                        printf("Tag name exceeded limit\n");
                        exit(-1);
                    }
                }

                if (ptag[0] != '\0') {
                    char *replace_string = mustache_get(m, ptag);
                    if (replace_string != NULL) {
                        text_parsed_add_string(m, replace_string);
                    }
                }

                free(ptag);
                ptag = NULL;
            }
            else {
                pos_dec(m);
                text_parsed_add_char(m, &c);
            }
        }
        else {
            text_parsed_add_char(m, &c);
        }
    }
}

void mustache_load_txt(pmustache m, char *data) {
    text_set(m, data);
    text_set_size(m);
}

char *mustache_get(pmustache m, char *key) {
    for (int i = 0;i < m->tags_values_index;i++) {
        if (strcmp(m->tags_values[i], key) == 0) {
            return m->tags_values[i + 1];
        }
    }

    return NULL;
}

void mustache_set(pmustache m, char *key, char *value) {
    m->tags_values[m->tags_values_index] = key;
    m->tags_values_index++;

    m->tags_values[m->tags_values_index] = value;
    m->tags_values_index++;
}


//<editor-fold defaultstate="collapsed" desc="Position functions">

int pos_get(pmustache m) {
    return m->text_position;
}

void pos_inc(pmustache m) {
    m->text_position++;
}

void pos_dec(pmustache m) {
    m->text_position--;
}

int pos_get_inc(pmustache m) {
    int x = pos_get(m);
    pos_inc(m);
    return x;
}
//</editor-fold>


// <editor-fold defaultstate="collapsed" desc="Text Functions">

char text_get_char(pmustache m) {
    if (pos_get(m) < text_get_size(m)) {
        return m->text[pos_get_inc(m)];
    }
    return EOF;
}

char text_get_char_pos(pmustache m, int n) {
    if (n < text_get_size(m)) {
        return m->text[n];
    }
    return EOF;
}

void text_set(pmustache m, char *text) {
    m->text = text;
}

void text_set_size(pmustache m) {
    if (m->text != NULL) {
        m->text_size = strlen(m->text);
    }
}

int text_get_size(pmustache m) {
    return m->text_size;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Text Parsed Functions">

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
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tag Functions">

bool tag_start(pmustache m, char *c) {
    return tag_char("start_first", m, c);
}

bool tag_start_last(pmustache m, char *c) {
    return tag_char("start_last", m, c);
}

bool tag_end(pmustache m, char *c) {
    return tag_char("end_first", m, c);
}

bool tag_end_last(pmustache m, char *c) {
    return tag_char("end_last", m, c);
}

bool tag_char(char *key, pmustache m, char *c) {
    char tag_c = '\0';

    if (strcmp(key, "start_first") == 0) {
        tag_c = m->start_first_char;
    }
    else if (strcmp(key, "start_last") == 0) {
        tag_c = m->start_last_char;
    }
    else if (strcmp(key, "end_first") == 0) {
        tag_c = m->end_first_char;
    }
    else if (strcmp(key, "end_last") == 0) {
        tag_c = m->end_last_char;
    }

    if (tag_c == '\0') {
        printf("Error on tag select char\n");
        exit(-1);
    }

    return (tag_c == *c);
}

bool is_tag(pmustache m, char *c) {
    if (tag_start(m, &c)) {
        if (tag_start_last(m, (char[]) {text_get_char(m)})) {
            int start_position = pos_get(m);

            tag_read_to_end(m);

            int end_position = pos_get(m);
            int tag_len = end_position - start_position;

            if (tag_len > MUSTACHE_TAGS_MAX_LEN_SIZE) {
                printf("Tag exceeds tag length limit\n");
                exit(-1);
            }
        }
    }

    /*

            //Default value tag
            char *ptag = calloc(MUSTACHE_TAGS_MAX_LEN_SIZE, sizeof (char));
            int tag_index = 0;


            while ((tag_char = text_get_char(m))) {

                //if (escape_values == false)

                if (tag_end(m, &tag_char)) {
                    char tmp_char = text_get_char(m);
                    if (tag_end_last(m, &tmp_char)) {
                        break;
                    }
                    else {
                        pos_dec(m);
                    }
                }

                ptag[tag_index] = tag_char;
                tag_index++;

                if (tag_index > MUSTACHE_TAGS_MAX_LEN_SIZE) {
                    printf("Tag name exceeded limit\n");
                    exit(-1);
                }
            }

            if (ptag[0] != '\0') {
                char *replace_string = mustache_get(m, ptag);
                if (replace_string != NULL) {
                    text_parsed_add_string(m, replace_string);
                }
            }

            free(ptag);
            ptag = NULL;
        }
            else {
                pos_dec(m);
                text_parsed_add_char(m, &c);
            }
        }
     */
}

void tag_read_to_end(pmustache m) {
    char c;

    while ((c = text_get_char(m))) {
        switch (c) {
            case '\n':
            case ' ':
            case EOF:
                break;
            default:
                if (tag_start(m, c)) {
                    break;
                }
        }
    }
}

//</editor-fold>

char *text_escape(char *string) {
    if (string != NULL) {
        int len = strlen(string);

        int escape_size = len * 2;
        int escaped_index = 0;
        char *escaped_text = calloc(escape_size, sizeof (char));

        int index = 0;

        int add = 0;
        char *to_add = NULL;

        while (string[index] != '\0') {

            switch (string[index]) {
                case '&':
                    to_add = "&amp;";
                    add = 5;
                    break;
                case '\"':
                    to_add = "&quot;";
                    add = 6;
                    break;
                case '\'':
                    to_add = "&apos;";
                    add = 6;
                    break;
                case '<':
                    to_add = "&lt;";
                    add = 4;
                    break;
                case '>':
                    to_add = "&gt;";
                    add = 4;
                    break;
                default:
                    to_add = &(string[index]);
                    add = 1;
            }

            if (add > 1) {
                if ((escaped_index + add) > escape_size - 1) {
                    escape_size += add * 2;

                    char *tmp_buffer = realloc(escaped_text, escape_size * sizeof (char));
                    if (tmp_buffer != NULL) {
                        escaped_text = tmp_buffer;
                    }
                    else {
                        printf("Unable to allocate memory for tmp_buffer\n");
                        exit(-1);
                    }
                }

                strcat(escaped_text, to_add);
                escaped_index += add;
            }
            else {
                escaped_text[escaped_index++] = *to_add;
            }

            index++;
        }

        return escaped_text;
    }

    return NULL;

}