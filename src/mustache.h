
#ifndef MUSTACHE_H
#define	MUSTACHE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MUSTACHE_TAGS_MAX_LEN_SIZE 255
#define MUSTACHE_TAGS_TOTAL 150

    typedef struct {
        char *text;
        int text_position;
        int text_size;

        char *text_parsed;
        int text_parsed_position;
        int text_parsed_size;

        char start_first_char;
        char start_last_char;
        char end_first_char;
        char end_last_char;

        //For now this will be an array of arrays containing the tags and values in the following away
        //<tag>, <value>, <tag2>, <value2>, etc...
        //This will be double the size of MUSTACHE_TAGS_TOTAL
        //set in mustache_init
        char **tags_values;
        int tags_values_size;
        int tags_values_index;

    } mustache, *pmustache;


    pmustache mustache_init();

    void mustache_load_txt(pmustache, char *);
    void mustache_load_file(pmustache, char *);

    void mustache_set(pmustache, char *,char *);
    char *mustache_get(pmustache, char *);

    void mustache_render(pmustache);


#ifdef	__cplusplus
}
#endif

#endif	/* MUSTACHE_H */

