mustache
========

C implementation of the [mustache templating language](http://mustache.github.io/)

**Supported features**

 - Normal variable tags
 - Not escaped variable tags
 - Comments
 - Sections
 - Inverted Sections

**Example of usage**

    #include <stdio.h>
    #include <stdlib.h>
    #include "src/mustache.h"


    char example_text[] = "Hello {{name}}\n"
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

        pmustache m = mustache_init();

        mustache_set(m, "name", "User");
        mustache_set(m, "value", "123.33");
        mustache_set(m, "taxed_value", "50");
        //mustache_set(m, "in_ca", "true");

        mustache_load_txt(m, example_text);

        mustache_render(m);

        printf("Rendered text: %s\n", m->text_parsed);

        return (EXIT_SUCCESS);
    }
