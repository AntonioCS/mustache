#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> //getcwd
#include <limits.h> //PATH_MAX


#ifndef MUSTACHE_H
#define	MUSTACHE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MUSTACHE_TAGS_MAX_LEN_SIZE 255
#define MUSTACHE_TAGS_TOTAL 150
#define MUSTACHE_TAGS_DEFAULT_START_FIRST_CHAR '{'
#define MUSTACHE_TAGS_DEFAULT_START_LAST_CHAR '{'
#define MUSTACHE_TAGS_DEFAULT_END_FIRST_CHAR '}'
#define MUSTACHE_TAGS_DEFAULT_END_LAST_CHAR '}'

#define MUSTACHE_TAGS_TYPE_NOESCAPE '{'
#define MUSTACHE_TAGS_TYPE_NOESCAPE_ALTERNATIVE '&'
#define MUSTACHE_TAGS_TYPE_SECTION '#'
#define MUSTACHE_TAGS_TYPE_SECTION_INVERTED '^'
#define MUSTACHE_TAGS_TYPE_SECTION_CLOSING '/'
#define MUSTACHE_TAGS_TYPE_COMMENT '!'
#define MUSTACHE_TAGS_TYPE_PARTIAL '>'
#define MUSTACHE_TAGS_TYPE_DELIMITER '='
#define MUSTACHE_TAGS_PARTIAL_EXT "mustache"   

    typedef struct {
        char *text;
        size_t text_position;
        size_t text_size;

        char *text_parsed;
        size_t text_parsed_position;
        size_t text_parsed_size;

        //These will be set to the default chars on mustache_init()
        char start_first_char;
        char start_last_char;
        char end_first_char;
        char end_last_char;

        //For now this will be an array of arrays containing the tags and values in the following away
        //<tag>, <value>, <tag2>, <value2>, etc...
        //This will be double the size of MUSTACHE_TAGS_TOTAL
        //set in mustache_init
        char **tags_values;
        size_t tags_values_size;
        size_t tags_values_index;

        //Extension for partials
        char *partial_ext;
        //Directory where to look for partial
        char *partial_dir; //This will be set to the current directory
        
        
        //source text function
        char (*text_get_char)(pmustache);
        char (*text_get_char_pos)(pmustache, int);
        
        
    } mustache, *pmustache;


    pmustache mustache_init(void);

    void mustache_load_text(pmustache, char *);
    void mustache_load_file(pmustache, char *);

    void mustache_set(pmustache, char *,char *);
    //Pass an array to loop through to then call mustache_set
    void mustache_sets(pmustache, char *, int);
    char *mustache_get(pmustache, char *);

    void mustache_render(pmustache);

    //@TODO Implement mustache_free
    void mustache_free(pmustache);


#ifdef	__cplusplus
}
#endif

#endif	/* MUSTACHE_H */

