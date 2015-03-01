#include "../inc/tags.h"
#include "../inc/text.h"
#include "../inc/position.h"
#include "../inc/util.h"
#include "../inc/text_parsed.h"
#include <unistd.h>  //getcwd

#define MUSTACHE_TAGS_DEBUG

static char *tag_error = NULL;

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
        printf("Error on tag select char for %s \n", key);
        //perror("Error on tag select char for %s \n", key);
        return false;
        //exit(-1);
    } else {
        return (tag_c == *c);
    }
}

bool is_tag(pmustache m, char *c) {
    if (tag_start(m, c)) {
#if defined(MUSTACHE_TAGS_DEBUG)
        printf("Tag DEBUG: Has tag char\n");
#endif
        if (tag_start_last(m, (char[]) { m->text_get_char(m)})) {
#if defined(MUSTACHE_TAGS_DEBUG)
        printf("Tag DEBUG: Has tag second char\n");
#endif

        int start_position = m->pos_get(m);
        //printf("The start position: %d\n", start_position);
        char tag_type = m->text_get_char_pos(m, start_position);
        //printf("Tag type is: %c\n", tag_type);

        int tag_read_end_type;

        while (true) {
            tag_read_end_type = tag_read_to_end(m);

            //to allow multiline comments
            if (tag_read_end_type == MUSTACHE_TAGS_END_BREAKLINE
                    && tag_type == MUSTACHE_TAGS_TYPE_COMMENT) {
                continue;
            }

            break;
        }

        int end_position = m->pos_get(m);
        int tag_len = end_position - start_position;

#if defined(MUSTACHE_TAGS_DEBUG)
        printf("Tag DEBUG: Start position: %d\nEnd Position: %d\n", start_position, end_position);
        printf("Tag DEBUG: Chars at start positions: %c\n", m->text_get_char_pos(m, start_position));
        printf("Tag DEBUG: Chars at positions: %c - %c\n",
                m->text_get_char_pos(m, end_position - 2), m->text_get_char_pos(m, end_position - 1));
        printf("Tag DEBUG: Tag read type: %d\n", tag_read_end_type);
#endif

        if (tag_read_end_type == MUSTACHE_TAGS_END_EOF_ZERO) {
            tag_set_error("Invalid tag: Tag has no end part");
            return false;
        }

        if ((tag_len - 2) > MUSTACHE_TAGS_MAX_LEN_SIZE
                && tag_type != MUSTACHE_TAGS_TYPE_COMMENT) {
            tag_set_error("Invalid tag: Tag exceeds tag length limit");
            return false;
        }

        char *tag = calloc(tag_len, sizeof (char));
        m->pos_set(m, start_position);

        for (int i = 0; i < (tag_len - 2); i++) {
            tag[i] = m->text_get_char(m);
        }
        m->pos_set(m, end_position);

        if (tag[0] == '\0') {
            perror("Invalid tag: Unable to read tag\n");
            return false;
        }
#if defined(MUSTACHE_TAGS_DEBUG)
        else {
            printf("Tag DEBUG: %s\n", tag);
        }
#endif

        tag_handle(m, tag, start_position, end_position, tag_len);
        return true;

    } else {
            m->pos_dec(m);
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
    
#if defined(MUSTACHE_TAGS_DEBUG)
    printf("Tag DEBUG: Before tag_clean: %s\n", tinfo.tag);
#endif

    tag_clean(&tinfo);

#if defined(MUSTACHE_TAGS_DEBUG)    
    printf("Tag DEBUG: After tag_clean: %s\n", tinfo.tag);
#endif    

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

void tag_handle_variable(ptag_info ti) {
    tag_write_data(ti, true);
}

void tag_handle_no_escape(ptag_info ti) {
    //NOTE: this tag has a '}' (if I used the { because to escape text you can also use &)
    //besides the normal (if it hasn't been changed) '}}'
    //The tag_read_to_end will have stopped at the the second '}'
    //which in this case is the first end char.
    //I must set the current position to +1
    if (ti->tag_type == MUSTACHE_TAGS_TYPE_NOESCAPE) {
        ti->m->pos_inc(ti->m);
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
    int section_len = closet_data->position - ti->m->pos_get(ti->m);

    bool do_section = (state ? value != NULL : value == NULL);


    if (do_section) {
        //Create new mustache with the section string
        //Merge the current mustache data (there is no function to do this)
        //with this new instance
        //Render
        //Merge with current mustache
        pmustache section = mustache_init();

        char *section_text = malloc(sizeof (char) * section_len + 1);
        memset(section_text, '\0', section_len + 1);
        memcpy(section_text, ti->m->text + ti->m->pos_get(ti->m), section_len);

        mustache_load_text(section, section_text);

        //printf("Section text: %s\n", section_text);
        //exit(-1);

        for (int i = 0; i < ti->m->tags_values_index; i++) {
            //printf("Writing values: %d - %s\n", i, ti->m->tags_values[i]);
            section->tags_values[i] = ti->m->tags_values[i];
        }
        section->tags_values_index = ti->m->tags_values_index;


        mustache_render(section);
        text_parsed_add_string(ti->m, section->text_parsed);

        //This will not work properly. The mustache pointer has data inside it that must also be freed
        //free(section);
    }

    ti->m->pos_set(ti->m, closet_data->position + closet_data->len);

}

tag_end_data *tag_find_closing(ptag_info ti) {
    char tag_closing[MUSTACHE_TAGS_MAX_LEN_SIZE];
    memset(tag_closing, '\0', MUSTACHE_TAGS_MAX_LEN_SIZE);

    tag_closing[0] = ti->m->start_first_char;
    tag_closing[1] = ti->m->start_last_char;
    tag_closing[2] = MUSTACHE_TAGS_TYPE_SECTION_CLOSING;

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
    //printf("Tag delimiter: %s, %d\n", ti->tag, (int)strlen(ti->tag));
    if (strlen(ti->tag) < 4) {
        perror("Long length of tag delimiter");
        exit(-1);
    }

    ti->m->start_first_char = ti->tag[0];
    ti->m->start_last_char = ti->tag[1];
    ti->m->end_first_char = ti->tag[2];
    ti->m->end_last_char = ti->tag[3];
}

void tag_clean(ptag_info ti) {
    char clean_tag[ti->len];
    memset(clean_tag, '\0', ti->len);

    if (ti->tag_type != MUSTACHE_TAGS_TYPE_COMMENT) {

        int len_cpy = ti->len - 1;
        int i = 0;


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
                break;
        }

        //n is necessary because in case there is space in tag
        //I will increase i but n must still be the same
        for (int n = 0; i < len_cpy; i++) {
            if (!isspace(ti->tag[i])) {
                clean_tag[n] = ti->tag[i];
                n++;
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

    while ((c = m->text_get_char(m))) {
        switch (c) {
            case '\n': //in case of comment this should not return
                return MUSTACHE_TAGS_END_BREAKLINE;
            case '\0':
            case EOF:
                return MUSTACHE_TAGS_END_EOF_ZERO;
            default:
                if (tag_end(m, &c)) {
                    char next_c = m->text_get_char(m);

                    if (tag_end_last(m, &next_c)) {
                        return MUSTACHE_TAGS_END_CLOSING_CHARS;
                    } else {
                        //When text_get_char returns EOF the position is not incremented
                        //so we must check here otherwise an infinite loop will be generated
                        if (next_c == EOF) {
                            return MUSTACHE_TAGS_END_EOF_ZERO;
                        }

                        m->pos_dec(m);
                    }
                }
        }
    }

    return MUSTACHE_TAGS_END_ABNORNAL_TERMINATION; //should not reach this
}

void tag_write_data(ptag_info ti, bool escape) {
    char *replace_string = mustache_get(ti->m, ti->tag);

    if (replace_string != NULL) {
        if (escape) {
            replace_string = text_escape(replace_string);
        }
        printf("Value found for tag %s: %s\n", ti->tag, replace_string);
        text_parsed_add_string(ti->m, replace_string);
    } else {
        printf("No value found for: %s\n", ti->tag);
    }

    printf("--------- Position: %d\n",(int)ti->m->pos_get(ti->m));
}


void tag_set_error(char *emsg) {
    tag_error = emsg;
}

bool tag_has_error() {
    return (tag_error == NULL);
}

//</editor-fold>