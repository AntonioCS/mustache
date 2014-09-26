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
#include "src/mustache_get_contents.h"

char text_tag_variable[] = "Hello {{name}}";
char text_tag_variable_no_escape[] = "Escaped html: {{html}} ---- Not escaped html: {{{html}}}";
char text_tag_variable_no_escape_andsign[] = "Escaped html: {{html}} ---- Not escaped html: {{&html}}";
char text_tag_comment[] = "<h1>Today{{! ignore me }}.</h1>";
char text_tag_comment_multiline[] = "<h1>Today</h1> {{! ignore me\n"
                                    " This is still a commnent\n"
                                    " This is still a commnent2\n"
                                    " This is still a commnent3}}\n"
                                    "This should be seen -> {{ name}}";
char text_tag_variable_error[] = "Hello {{name}"; //missing end }
char text_tag_variable_error2[] = "Hello {name}}"; //Missing second {. This will not generate an error the parser is suppose to just show everything


char text_tag_section[] = "Hello {{name}}\n"
               "You have just won {{value}} dollars!\n"
               "{{#in_ca}}"
               "Well, {{taxed_value}} dollars, after taxes.\n"
               "{{/in_ca}}"
               "The End"
        ;

char text_tag_section_inverted[] = "Hello {{name}}\n"
               "You have just won {{value}} dollars!\n"
               "{{#in_ca}}"
               "Well, {{taxed_value}} dollars, after taxes.\n"
               "{{/in_ca}}"
               "{{^in_ca}}"
               "No tax!!\n"
               "{{/in_ca}}"
               "The End"
        ;

int main(int argc, char** argv) {
/*
    pmustache m = mustache_init();
    char *text = NULL;
    //text = text_tag_variable;
    //text = text_tag_variable_no_escape;
    //text = text_tag_variable_no_escape_andsign;
    //text = text_tag_comment;
    //text = text_tag_comment_multiline;
    //text = text_tag_variable_error;
    //text = text_tag_variable_error2;
    //text = text_tag_section;
    text = text_tag_section_inverted;

    mustache_set(m, "name", "António");
    mustache_set(m, "value", "123.33");
    mustache_set(m, "taxed_value", "50");
    mustache_set(m, "html", "<script type=\"text/javascript\" src=\"acs.js\" />");
    //mustache_set(m, "in_ca", "true");

    mustache_load_txt(m, text);

    mustache_render(m);

    printf("Rendered text: %s\n", m->text_parsed);
*/
    char *text = NULL;

    text = mustache_get_contents("src/tpl.mustache");
    //text = mustache_get_contents("src/tpl_large.mustache");
    printf("Text: %s\n", text);
    free(text);


    return (EXIT_SUCCESS);
}