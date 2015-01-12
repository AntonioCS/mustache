#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"

int tests_run = 0;
int asserts_run = 0;

#include "../src/position.c"
#include "../src/text.c"

static pmustache m;

static char *test_text_get_char() {
    m->text = "Hello";
    m->text_size = 5;
    m->text_position = 0;
        
    char expected_char = 'H';
    
    mu_assert("error, test_text_get_char did not return correct char", text_get_char(m) == expected_char);
    return 0;
}

static char *test_text_get_char_pos() {    
    m->text = "Hello";
    m->text_size = 5;
    
    char expected_char = 'e';
    int position = 1;
    
    mu_assert("error, text_get_char_pos did not return correct char", text_get_char_pos(m, position) == expected_char);
    
    //out side limit
    position = 20;
    mu_assert("error, text_get_char_pos did not return EOF", text_get_char_pos(m, position) == EOF);
    return 0;
}

static char *test_text_set() {    
    text_set(m, "Hello");
            
    mu_assert("error, text_set did not set the text correctly", (strcmp(m->text, "Hello") == 0));
    return 0;
}

static char *test_text_set_size() {
    m->text = "Hello";    
    text_set_size(m);        
    
    mu_assert("error, text_set_size set incorrect size", m->text_size == 5);    
    return 0;
}

static char *test_text_get_size() {
    m->text_size = 10;    
    
    mu_assert("error, text_get_size incorrect size", text_get_size(m) == 10);    
    return 0;
}


static char *all_tests() {
    m = malloc(sizeof (mustache));    

    mu_run_test(test_text_get_char);
    mu_run_test(test_text_get_char_pos);
    mu_run_test(test_text_set);    
    mu_run_test(test_text_set_size);
    mu_run_test(test_text_get_size);
    
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

