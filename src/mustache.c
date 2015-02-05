#include "../inc/mustache.h"
#include "../inc/tags.h"
#include "../inc/position.h"
#include "../inc/text.h"
#include "../inc/text_parsed.h"

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
    
    m->partial_ext = MUSTACHE_TAGS_PARTIAL_EXT;    
    
    m->partial_dir = calloc(PATH_MAX, sizeof(char));
    if (m->partial_dir != NULL && getcwd(m->partial_dir, PATH_MAX) != NULL) {
        strcat(m->partial_dir, "/");
    }
    
    return m;
}

void mustache_render(pmustache m) {
    char c;

    if (text_parsed_init(m) != true) {
        perror("Unable to initialize text_parsed\n");
        exit(-1);
    }

    while ((c = m->text_get_char(m)) != EOF) {

        //Avoid passing spaces to is_tag
        if (isspace(c) || is_tag(m, &c) == false) {
            text_parsed_add_char(m, &c);
        }

    } //while
}

void mustache_load_text(pmustache m, char *data) {
    m->text_get_char = text_get_char;
    m->text_get_char_pos = text_get_char_pos;
    
    text_set(m, data);
    text_set_size(m);
}

void mustache_load_file(pmustache m, char *file) {
    m->text_get_char = text_file_get_char;
    m->text_get_char_pos = text_file_get_char_pos;
    
    //text_set(m, data);
    //text_set_size(m);
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
