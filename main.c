/*
 * File:   main.c
 * Author: AntonioCS
 *
 * Created on 7 de Setembro de 2014, 17:24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYWORDS_MAX_SIZE 255
#define KEYWORDS_TOTAL 150

char text[] = "Hello {{name}}!\n"
        "You have just won {{value}} dollars!"
        "Thanks for playing {{name}} {{cona}}."
    ;

char key_values[][50] = {
    "name", "Antonio", "value", "123.33"
};



char *new_text;

int new_text_index = 0;

int position = 0;
int pos_get();
void pos_inc();
void pos_dec();
int get_pos_inc();

int text_size;// = strlen(text);

void set_text_size();
int get_text_size();
char get_char();
void add_char(char *);

int keywords_index = 0;

int main(int argc, char** argv) {

    set_text_size();

    //For now make it just 3 times bigger than the original
    new_text = calloc(get_text_size() * 3, sizeof (char));


    char start_char = '{';
    char second_start_char = '{';

    char last_end_char = '}';
    char first_end_char = '}';
    char x;

    char *keywords[KEYWORDS_TOTAL] = {NULL};

    for (int n = 0;n < KEYWORDS_TOTAL;n++) {
        keywords[n] = malloc(sizeof (char*));
    }

    while ((x = get_char()) != EOF) {

        if (x == start_char) {
            if (get_char() == second_start_char) {
                char *keyword = calloc(KEYWORDS_MAX_SIZE, sizeof (char));
                int keyword_index = 0;
                char keyword_char;

                while ((keyword_char = get_char())) {
                    if (keyword_char == first_end_char) {
                        if (get_char() == last_end_char) {
                            break;
                        }
                    }

                    keyword[keyword_index] = keyword_char;
                    keyword_index++;
                }

                if (keyword[0] != '\0') {
                    keywords[keywords_index] = keyword;
                    keywords_index++;

                    ///*
                    for (int i = 0, max = 3;i <= max;i += 2) {
                        if (strcmp(key_values[i], keyword) == 0) {
                            char *replace_string = key_values[i + 1];
                            int replace_len = strlen(replace_string);

                            printf("Replace string -> %s\n", replace_string);
                            for (int aa = 0;aa < replace_len;aa++) {
                                add_char(&replace_string[aa]);
                            }
                        }
                    }
                    //*/

                }
                else {
                    free(keyword);
                }
            }
            else {
                pos_dec();
            }
        }
        else {
            add_char(&x);
            //new_text[new_text_index] = x;
            //new_text_index++;
        }
    }

    if (keywords_index > 0) {
        for (int i = 0;i < keywords_index;i++) {
            printf("keyword = %s\n", keywords[i]);
        }
    }

    printf("New test: %s\n", new_text);
    return (EXIT_SUCCESS);
}

char get_char() {

    if (position < text_size) {
        return text[get_pos_inc()];
    }

    return EOF;
}

int pos_get() {
    return position;
}

void pos_inc() {
    position++;
}

void pos_dec() {
    position--;
}

int get_pos_inc() {
    int x = pos_get();
    pos_inc();
    return x;
}

void set_text_size() {
    text_size = strlen(text);
}

int get_text_size() {
    return text_size;
}

void add_char(char *x) {
    new_text[new_text_index] = *x;
    new_text_index++;
}