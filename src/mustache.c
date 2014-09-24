#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "mustache.h"


static int pos_get(pmustache);
static void pos_inc(pmustache);
static void pos_dec(pmustache);
static int pos_get_inc(pmustache);
static void pos_set(pmustache, int);

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

typedef struct {
    pmustache m;
    char *tag;
    int p_start;
    int p_end;
    int len;
    char tag_type;
} tag_info, *ptag_info;

typedef struct {
    char *pospointer;
    int position;
    int len;
} tag_end_data;

static void tag_handle(pmustache, char *, int, int, int);
static void tag_handle_variable(ptag_info);
static void tag_handle_no_escape(ptag_info);

static void tag_handle_sections(ptag_info);
static void tag_handle_sections_inverted(ptag_info);
static void tag_handle_sections_and_inverted_sections(ptag_info, bool);
static void tag_handle_comments(ptag_info);
static void tag_handle_partials(ptag_info);
static void tag_handle_delimiter(ptag_info);
static void tag_write_data(ptag_info, bool);
//This will be used to clean up the tags which just means that it will remove the first character when the tag is not a variable
static void tag_clean(ptag_info);
static tag_end_data *tag_find_closing(ptag_info);


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
    for (int n = 0;n < m->tags_values_size;n++) {
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

        if (isspace(c) || is_tag(m, &c) == false) {
            text_parsed_add_char(m, &c);
        }

    } //while
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

void pos_set(pmustache m, int p) {
    m->text_position = p;
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
        m->text_size = (int) strlen(m->text);
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
    if (tag_start(m, c)) {

        if (tag_start_last(m, (char[]) {text_get_char(m)})) {
            int start_position = pos_get(m);
            char tag_type = text_get_char_pos(m, start_position);
            int tag_read_end_type;

            while (true) {
                tag_read_end_type = tag_read_to_end(m);

                //to allow multiline comments
                if (tag_read_end_type == MUSTACHE_TAGS_END_BREAKLINE && tag_type == MUSTACHE_TAGS_TYPE_COMMENT) {
                    continue;
                }

                break;
            }

            int end_position = pos_get(m);
            int tag_len = end_position - start_position;
            /*
            printf("Start position: %d\nEnd Position: %d\n", start_position, end_position);
            printf("Chars at start positions: %c\n", text_get_char_pos(m, start_position));
            printf("Chars at positions: %c - %c\n", text_get_char_pos(m, end_position - 2), text_get_char_pos(m, end_position - 1));
            printf("Tag read type: %d\n", tag_read_end_type);
            //*/

            if (tag_read_end_type == MUSTACHE_TAGS_END_EOF_ZERO) {

                if (tag_end(m, (char[]) {text_get_char_pos(m, end_position - 2)}) || tag_end_last(m, (char[]) {text_get_char_pos(m, end_position - 1)})) {
                    printf("Invalid tag: Tag has no end part\n");
                    exit(-1);
                }
            }

            if ((tag_len - 2) > MUSTACHE_TAGS_MAX_LEN_SIZE && tag_type != MUSTACHE_TAGS_TYPE_COMMENT) {
                printf("Invalid tag: Tag exceeds tag length limit\n");
                exit(-1);
            }

            char *tag = calloc(tag_len, sizeof (char));
            for (int i = 0;i < (tag_len - 2);i++) {
                tag[i] = text_get_char_pos(m, start_position + i);
            }

            if (tag[0] == '\0') {
                printf("Invalid tag: Unable to read tag\n");
                return false;
            }
            else {
                //printf("Tag: %s\n", tag);
            }

            tag_handle(m, tag, start_position, end_position, tag_len);
            return true;

        }
        else {
            pos_dec(m);
        }
    }

    return false;
}

void tag_handle(pmustache m, char *tag, int p_start, int p_end, int len) {

    tag_info tinfo = {
        .m = m,
        .tag = tag,
        .p_start = p_start,
        .p_end = p_end,
        .len = len,
        .tag_type = tag[0]
    };
    printf("Before tag_clean: %s\n", tinfo.tag);
    tag_clean(&tinfo);
    printf("After tag_clean: %s\n", tinfo.tag);

    switch (tinfo.tag_type) {
            //do not escape characters
        case MUSTACHE_TAGS_TYPE_NOESCAPE: //will have an extra } as close char
        case MUSTACHE_TAGS_TYPE_NOESCAPE_ALTERNATIVE: //this will not have an extra }
            tag_handle_no_escape(&tinfo);
            break;

            //Sections
        case MUSTACHE_TAGS_TYPE_SECTION:
            tag_handle_sections(&tinfo);
            break;

            //Inverted Sections
        case MUSTACHE_TAGS_TYPE_SECTION_INVERTED:
            tag_handle_sections_inverted(&tinfo);
            break;

            //Comments
        case MUSTACHE_TAGS_TYPE_COMMENT:
            //tag_handle_comments(&tinfo);
            //Ignore
            break;

            //Partials (I believe this is basically a way to include other templates)
        case MUSTACHE_TAGS_TYPE_PARTIAL:
            tag_handle_partials(&tinfo);
            break;

            //Set Delimiter
        case MUSTACHE_TAGS_TYPE_DELIMITER:
            tag_handle_delimiter(&tinfo);
            break;

        default:
            tag_handle_variable(&tinfo);
    }
}

void tag_write_data(ptag_info ti, bool escape) {
    char *replace_string = mustache_get(ti->m, ti->tag);

    if (replace_string != NULL) {
        if (escape) {
            replace_string = text_escape(replace_string);
        }
        printf("Value found for tag %s: %s\n", ti->tag, replace_string);
        text_parsed_add_string(ti->m, replace_string);
    }
    else {
        printf("No value found for: %s\n", ti->tag);
    }
}

void tag_handle_variable(ptag_info ti) {
    tag_write_data(ti, true);
}

void tag_handle_no_escape(ptag_info ti) {
    //NOTE: this tag has a '}' (if I used the { because to escape text you can also use &) besides the normal (if it hasn't been changed) '}}'
    //The tag_read_to_end will have stopped at the the second '}' which in this canse is the first end char.
    //I must set the current position to +1
    if (ti->tag_type == MUSTACHE_TAGS_TYPE_NOESCAPE) {
        pos_inc(ti->m);
    }

    tag_write_data(ti, false);
}

void tag_handle_sections(ptag_info ti) {
    tag_handle_sections_and_inverted_sections(ti, true);
}

void tag_handle_sections_inverted(ptag_info ti) {
    tag_handle_sections_and_inverted_sections(ti, false);
}

void tag_handle_sections_and_inverted_sections(ptag_info ti, bool state) {
    char *value = mustache_get(ti->m, ti->tag);
    tag_end_data *closet_data = tag_find_closing(ti);
    int section_len = closet_data->position - pos_get(ti->m);

    bool do_section = (state ? value != NULL : value == NULL);


    if (do_section) {
        //Create new mustache with the section string
        //Merge the current mustache data (there is no function to do this) with this new instance
        //Render
        //Merge with current mustache
        pmustache section = mustache_init();


        char *section_text = malloc(sizeof (char) * section_len + 1);
        memset(section_text, '\0', section_len + 1);
        memcpy(section_text, ti->m->text + pos_get(ti->m), section_len);

        mustache_load_txt(section, section_text);

        //printf("Section text: %s\n", section_text);
        //exit(-1);

        for (int i = 0;i < ti->m->tags_values_index;i++) {
            //printf("Writing values: %d - %s\n", i, ti->m->tags_values[i]);
            section->tags_values[i] = ti->m->tags_values[i];
        }
        section->tags_values_index = ti->m->tags_values_index;


        mustache_render(section);
        text_parsed_add_string(ti->m, section->text_parsed);

        //This will not work properly. The mustache pointer has data inside it that must also be freed
        //free(section);
    }
    
    pos_set(ti->m, closet_data->position + closet_data->len);

}
tag_end_data *tag_find_closing(ptag_info ti) {
    char tag_closing[MUSTACHE_TAGS_MAX_LEN_SIZE];
    memset(tag_closing, '\0', MUSTACHE_TAGS_MAX_LEN_SIZE);

    tag_closing[0] = ti->m->start_first_char;
    tag_closing[1] = ti->m->start_last_char;
    tag_closing[2] = MUSTACHE_TAGS_CLOSING_CHAR;

    strcat(tag_closing, ti->tag);

    int len = strlen(tag_closing);

    tag_closing[len] = ti->m->end_first_char;
    tag_closing[len + 1] = ti->m->end_last_char;


    //printf("End tag: %s\n", tag_closing);
    char *tag_closing_pos = strstr(ti->m->text + ti->m->text_position, tag_closing);

    tag_end_data *te_data = malloc(sizeof (tag_end_data));

    te_data->len = 0;
    te_data->position = 0;
    te_data->pospointer = NULL;

    if (tag_closing_pos != NULL) {
        te_data->pospointer = tag_closing_pos;
        te_data->position = tag_closing_pos - ti->m->text;
        te_data->len = len + 2;
    }

    return te_data;
}
void tag_handle_comments(ptag_info ti) {
    //Do nothing
}

void tag_handle_partials(ptag_info ti) {

}

void tag_handle_delimiter(ptag_info ti) {

}

void tag_clean(ptag_info ti) {
    char clean_tag[ti->len];
    memset(clean_tag, '\0', ti->len);

    if (ti->tag_type != MUSTACHE_TAGS_TYPE_COMMENT) {

        int len_cpy = ti->len - 1;
        int i = 0;
        int dec = 0;


        //will have an extra } as close char
        if (MUSTACHE_TAGS_TYPE_NOESCAPE == ti->tag_type) {
            len_cpy--;
        }

        switch (ti->tag_type) {
            case MUSTACHE_TAGS_TYPE_NOESCAPE:
            case MUSTACHE_TAGS_TYPE_NOESCAPE_ALTERNATIVE:
            case MUSTACHE_TAGS_TYPE_SECTION:
            case MUSTACHE_TAGS_TYPE_SECTION_INVERTED:
            case MUSTACHE_TAGS_TYPE_PARTIAL:
            case MUSTACHE_TAGS_TYPE_DELIMITER:
                i = 1;
                dec = 1;
            break;
        }

        for (;i < len_cpy;i++) {
            if (!isspace(ti->tag[i])) {
                clean_tag[i - dec] = ti->tag[i];
            }
        }

    }


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
            case '\n': //in case of comment this should not return
                return MUSTACHE_TAGS_END_BREAKLINE;
            case '\0':
            case EOF:
                return MUSTACHE_TAGS_END_EOF_ZERO;
            default:
                if (tag_end(m, &c)) {
                    char next_c = text_get_char(m);

                    if (tag_end_last(m, &next_c)) {
                        return MUSTACHE_TAGS_END_CLOSING_CHARS;
                    }
                    else {
                        //When text_get_char returns EOF the position is not incremented
                        //so we must check here otherwise an infinite loop will be generated
                        if (next_c == EOF) {
                            return MUSTACHE_TAGS_END_EOF_ZERO;
                        }

                        pos_dec(m);
                    }
                }
        }
    }

    return -1;//should not reach this
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