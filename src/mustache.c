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
//All of the tag_start* and tag_end* functions will call this one
static bool tag_char(char *, pmustache, char *);
static int tag_read_to_end(pmustache m);
static bool is_tag(pmustache, char *);

typedef struct tag_info {
    pmustache m;
    char *tag;
    int p_start;
    int p_end;
    int len;
    char tag_type;
} tag_info, *ptag_info ;

void tag_handle(pmustache, char *, int, int, int);
void tag_handle_variable(ptag_info);
void tag_handle_no_escape(ptag_info);
void tag_handle_sections(ptag_info);
void tag_handle_sections_inverted(ptag_info);
void tag_handle_comments(ptag_info);
void tag_handle_partials(ptag_info);
void tag_handle_delimiter(ptag_info);
void tag_write_data(ptag_info, bool);
//This will be used to clean up the tags which just means that it will remove the first character when the tag is not a variable
void tag_clean(ptag_info);


static bool text_parsed_init(pmustache);
static void text_parsed_add_char(pmustache, char *);
static void text_parsed_add_string(pmustache m, char *);

static char *text_escape(const char *);

pmustache mustache_init() {
    pmustache m = malloc(sizeof (mustache));

    m->text = NULL;
    m->text_parsed = NULL;

    m->tags_values_index = 0;
    m->tags_values_size = (MUSTACHE_TAGS_TOTAL * 2);
    m->tags_values = malloc(sizeof (char *) * m->tags_values_size);
    for (int n = 0; n < m->tags_values_size; n++) {
        m->tags_values[n] = malloc(sizeof (char*));
    }

    m->start_first_char = MUSTACHE_TAGS_DEFAULT_START_FIRST_CHAR;
    m->start_last_char = MUSTACHE_TAGS_DEFAULT_START_LAST_CHAR;
    m->end_first_char = MUSTACHE_TAGS_DEFAULT_END_FIRST_CHAR;
    m->end_last_char = MUSTACHE_TAGS_DEFAULT_END_LAST_CHAR;

    m->text_parsed_position = 0;
    m->text_size = 0;

    m->text_parsed_position = 0;
    m->text_parsed_size = 0;

    return m;
}

void mustache_render(pmustache m) {
    char c;    

    if (text_parsed_init(m) != true) {
        printf("Unable to initialize text_parsed\n");
        exit(-1);
    }

    while ((c = text_get_char(m)) != EOF) {

        if (is_tag(m, &c) == false) {
            text_parsed_add_char(m, &c);
        }
        
    } //while
}

void mustache_load_txt(pmustache m, char *data) {
    text_set(m, data);
    text_set_size(m);
}

char *mustache_get(pmustache m, char *key) {
    for (int i = 0; i < m->tags_values_index; i++) {
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
    } else if (strcmp(key, "start_last") == 0) {
        tag_c = m->start_last_char;
    } else if (strcmp(key, "end_first") == 0) {
        tag_c = m->end_first_char;
    } else if (strcmp(key, "end_last") == 0) {
        tag_c = m->end_last_char;
    }

    if (tag_c == '\0') {
        printf("Error on tag select char\n");
        exit(-1);
    }

    return (tag_c == *c);
}

bool is_tag(pmustache m, char *c) {
    if (tag_start(m, c)) {

        if (tag_start_last(m, (char[]) {text_get_char(m)})) {
            int start_position = pos_get(m);

            int tag_read_type = tag_read_to_end(m);

            int end_position = pos_get(m);
            int tag_len = end_position - start_position;

            printf("Start position: %d\nEnd Position: %d\n", start_position, end_position);
            printf("Chars at positions: %c - %c\n", text_get_char_pos(m, end_position - 2), text_get_char_pos(m, end_position - 1));
            printf("Tag read type: %d\n", tag_read_type);
            
            //Last read char was space, newline or EOF
            if (tag_read_type == 1) {
                if (tag_end(m, (char[]) {text_get_char_pos(m, end_position - 2) }) || tag_end_last(m, (char[]) {text_get_char_pos(m, end_position - 1)})) {
                    return false;
                }
            }

            if ((tag_len - 2) > MUSTACHE_TAGS_MAX_LEN_SIZE) {
                printf("Tag exceeds tag length limit\n");
                exit(-1);
            }

            char *tag = calloc(tag_len, sizeof (char));

            for (int i = 0; i < (tag_len - 2); i++) {
                tag[i] = text_get_char_pos(m, start_position + i);
            }

            if (tag[0] == '\0') {
                printf("Unable to read tag\n");                
                return false;
            }
            else {
                printf("Tag: %s\n", tag);
            }

            tag_handle(m, tag, start_position, end_position, tag_len);
            return true;

        } else {
            pos_dec(m);
        }
    }

    return false;
}

void tag_handle(pmustache m, char *tag, int p_start, int p_end, int len) {
    //char tag_type = text_get_char(m);
    char tag_type = tag[0];
    
    tag_info tinfo = {
        .m = m,
        .tag = tag,
        .p_start = p_start,
        .p_end = p_end,
        .len = len,
        .tag_type = tag_type
    };    

    switch (tag_type) {
        //do not escape characters
        case '{': //will have an extra } as close char
            tag_handle_no_escape(&tinfo);
            break;
            
        //Sections
        case '#':
            tag_handle_sections(&tinfo);
            break;
            
        //Inverted Sections
        case '^':
            tag_handle_sections_inverted(&tinfo);
            break;

        //Comments
        case '!':
            tag_handle_comments(&tinfo);
            break;

        //Partials (I believe this is basically a way to include other templates)
        case '>':
            tag_handle_partials(&tinfo);
            break;

        //Set Delimiter
        case '=':
            tag_handle_delimiter(&tinfo);
            break;

        default:
            tag_handle_variable(&tinfo);            
    }

    //free(tag);
}

void tag_write_data(ptag_info ti, bool escape) {
    char *replace_string = mustache_get(ti->m, ti->tag);
    
    if (replace_string != NULL) {
        if (escape) {
            replace_string = text_escape(replace_string);
        }
        text_parsed_add_string(ti->m, replace_string);        
    }     
}

void tag_handle_variable(ptag_info ti) {
    tag_write_data(ti, true);    
}
void tag_handle_no_escape(ptag_info ti) {    
    tag_clean(ti);
    //NOTE: this tag has a '}' besides the normal (if it hasn't been changed) '}}'
    //The tag_read_to_end will have stopped at the the second '}' which in this canse is the first end char.
    //I must set the current position to +1
    pos_inc(ti->m);
    tag_write_data(ti, false);
}

void tag_handle_sections(ptag_info ti) {
    
}

void tag_handle_sections_inverted(ptag_info ti) {
    
}
void tag_handle_comments(ptag_info ti) {
    
}
void tag_handle_partials(ptag_info ti) {
    
}
void tag_handle_delimiter(ptag_info ti) {
    
}

void tag_clean(ptag_info ti) {
    char clean_tag[ti->len];
    memset(clean_tag, '\0', ti->len);
    strncpy(clean_tag, ti->tag+1, ti->len -1);
    
    memset(ti->tag, '\0', ti->len);
    strcat(ti->tag, clean_tag);    
}

/**
 * Continue reading until end of tag
 * 
 * @param m
 * @return 
 */
int tag_read_to_end(pmustache m) {
    char c;

    while ((c = text_get_char(m))) {
        switch (c) {
            case '\0':
            case '\n': //in case of comment this should not return
            //case ' ':
            case EOF:
                return 1;
            default:
                if (tag_end(m, &c)) {            
                    char x = text_get_char(m);
                    //if (tag_end_last(m, ((char[]) {text_get_char(m)}))) {
                    if (tag_end_last(m, &x)) {
                        printf("Found end tag!!!\n");
                        return 0;
                    } else {
                        pos_dec(m);
                    }
                }
        }
    }

    return -1; //should not reach this
}

//</editor-fold>

/**
 * Escape characters (change them) &, ", ', < and >
 * 
 * @param string Text 
 * @return String with text replaced
 */
char *text_escape(const char *string) {
    if (string != NULL) {
        int len = strlen(string);

        int escape_size = len * 2;
        int escaped_index = 0;
        char *escaped_text = calloc(escape_size, sizeof (char));

        int index = 0;

        int add = 0;
        const char *to_add = NULL;

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
                    } else {
                        printf("Unable to allocate memory for tmp_buffer\n");
                        exit(-1);
                    }
                }

                strcat(escaped_text, to_add);
                escaped_index += add;
            } else {
                escaped_text[escaped_index++] = *to_add;
            }

            index++;
        }

        return escaped_text;
    }

    return NULL;

}