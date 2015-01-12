/* 
 * File:   test_position.c
 * Author: antoniocs
 *
 * Created on 28 de Dezembro de 2014, 12:05
 */

#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"

int tests_run = 0;
int asserts_run = 0;

#include "../src/position.c"

static pmustache m;

static char *text_pos_get() {
    m->text_position = 0;
    mu_assert("error, position not equal to zero ", pos_get(m) == 0);
    return 0;
}

static char *text_pos_inc() {
    pos_inc(m);
    mu_assert("error, position was not incremented", m->text_position == 1);
    return 0;
}

static char *text_pos_dec() {
    m->text_position = 1;    
    pos_dec(m);    
    mu_assert("error, position was not decremented", m->text_position == 0);
    return 0;
}

static char *text_pos_get_inc() {
    m->text_position = 0;    
    
    mu_assert("error, position get inc not correct", pos_get_inc(m) == 0);
    mu_assert("error, position get inc not correct 2", m->text_position == 1);
    return 0;
}

static char *all_tests() {
    m = malloc(sizeof (mustache));
    m->text_position = 0;

    mu_run_test(text_pos_get);
    mu_run_test(text_pos_inc);
    mu_run_test(text_pos_dec);
    mu_run_test(text_pos_get_inc);
    
    return 0;
}

int main(int argc, char** argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

