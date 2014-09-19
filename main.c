/*
 * File:   main.c
 * Author: AntonioCS
 *
 * Created on 7 de Setembro de 2014, 17:24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/mustache.h"

char text_tag_variable[] = "Hello {{name}}";
char text_tag_variable_no_escape[] = "Escaped html: {{html}} ---- Not escaped html: {{{html}}}";

/*
char text[] = "Hello {{name}}\n"
               "You have just won {{value}} dollars!\n"
               "Escaped html: {{html}} ---- Not escaped html: {{{html}}}!\n"
               "{{#in_ca}}"
               "Well, {{taxed_value}} dollars, after taxes.\n"
               "{{/in_ca}}}"
        ;
*/
int main(int argc, char** argv) {

    pmustache m = mustache_init();
    
    //char *text = text_tag_variable;
    char *text = text_tag_variable_no_escape;
    
    mustache_set(m, "name", "António");
    mustache_set(m, "value", "123.33");
    mustache_set(m, "html", "<script type=\"text/javascript\" src=\"acs.js\" />");

    mustache_load_txt(m, text);

    mustache_render(m);

    printf("Rendered text: %s\n", m->text_parsed);

    return (EXIT_SUCCESS);
}