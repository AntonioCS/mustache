#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"

int tests_run = 0;
int asserts_run = 0;

#include "../src/text.c"
#include "../src/text_file.c"
#include "../src/position.c"
#include "../src/util.c"
#include "../src/text_parsed.c"
#include "../src/mustache.c"
#include "../src/tags.c"

static pmustache m;

static void set_default_data(pmustache m) {
    //defined in mustache.h
    m->start_first_char = MUSTACHE_TAGS_DEFAULT_START_FIRST_CHAR;
    m->start_last_char = MUSTACHE_TAGS_DEFAULT_START_LAST_CHAR;
    m->end_first_char = MUSTACHE_TAGS_DEFAULT_END_FIRST_CHAR;
    m->end_last_char = MUSTACHE_TAGS_DEFAULT_END_LAST_CHAR;
    
    m->text = "{{tag}}";
    m->text_position = 0;
    m->text_size = 7;
}

static char *test_tag_start() {
    char c = '{';

    mu_assert("error, tag_start failed", tag_start(m, &c) == true);
    return 0;
}

static char *test_tag_start_last() {
    char c = '{';

    mu_assert("error, tag_start_last failed", tag_start_last(m, &c) == true);
    return 0;
}

static char *test_tag_end() {
    char c = '}';

    mu_assert("error, tag_end failed", tag_end(m, &c) == true);
    return 0;
}

static char *test_tag_end_last() {
    char c = '}';

    mu_assert("error, tag_end_last failed", tag_end_last(m, &c) == true);
    return 0;
}

//All of the tag_start* and tag_end* functions will call this one

static char *test_tag_char() {
    return 0;
}

static char *test_is_tag() {
    set_default_data(m);
    
    char c = m->text[0];
    
    
    mu_assert("error, is_tag failed", is_tag(m, &c) == true);
    
    return 0;
}

static char *test_tag_handle() {
    return 0;
}

static char *test_tag_handle_variable() {
    return 0;
}

static char *test_tag_handle_no_escape() {
    return 0;
}

static char *test_tag_handle_sections() {
    return 0;
}

static char *test_tag_handle_sections_inverted() {
    return 0;
}

static char *test_tag_handle_sections_and_inverted_sections() {
    return 0;
}

static char *test_tag_handle_comments() {
    return 0;
}

static char *test_tag_handle_partials() {
    return 0;
}

static char *test_tag_handle_delimiter() {
    return 0;
}

static char *test_tag_write_data() {
    return 0;
}
//This will be used to clean up the tags which just means that it will remove the first character when the tag is not a variable

static char *test_tag_clean() {
    return 0;
}

static char *test_tag_find_closing() {
    return 0;
}

static char *test_tag_read_to_end() {
    return 0;
}

static char *all_tests() {
    m = malloc(sizeof (mustache));
    set_default_data(m);

    mu_run_test(test_tag_start);


    mu_run_test(test_tag_start_last);

    mu_run_test(test_tag_end);
    mu_run_test(test_tag_end_last);

    mu_run_test(test_tag_char);
    mu_run_test(test_tag_read_to_end);
    mu_run_test(test_is_tag);

    mu_run_test(test_tag_handle);
    mu_run_test(test_tag_handle_variable);
    mu_run_test(test_tag_handle_no_escape);

    mu_run_test(test_tag_handle_sections);
    mu_run_test(test_tag_handle_sections_inverted);
    mu_run_test(test_tag_handle_sections_and_inverted_sections);
    mu_run_test(test_tag_handle_comments);
    mu_run_test(test_tag_handle_partials);
    mu_run_test(test_tag_handle_delimiter);
    mu_run_test(test_tag_write_data);

    mu_run_test(test_tag_clean);
    mu_run_test(test_tag_find_closing);


    return 0;
}

int main(int argc, char** argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\nAsserts executed: %d\n", tests_run, asserts_run);

    return result != 0;
}