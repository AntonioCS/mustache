/* 
 * File:   tags.h
 * Author: antoniocs
 *
 * Created on 23 de Novembro de 2014, 14:00
 */
#include "mustache.h"

#ifndef TAGS_H
#define	TAGS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//For the tag_read_to_end function. To represent the various return values
#define MUSTACHE_TAGS_END_BREAKLINE 2
#define MUSTACHE_TAGS_END_EOF_ZERO 1
#define MUSTACHE_TAGS_END_CLOSING_CHARS 0
#define MUSTACHE_TAGS_END_ABNORNAL_TERMINATION -1
    
    typedef struct {
        pmustache m;
        char *tag;
        size_t p_start;
        size_t p_end;
        size_t len;
        char tag_type;
    } tag_info, *ptag_info;

    typedef struct {
        char *pospointer;
        size_t position;
        size_t len;
    } tag_end_data;

    bool tag_start(pmustache, char *);
    bool tag_start_last(pmustache, char *);
    bool tag_end(pmustache, char *);
    bool tag_end_last(pmustache, char *);
    //All of the tag_start* and tag_end* functions will call this one
    bool tag_char(char *, pmustache, char *);
    int tag_read_to_end(pmustache m);
    bool is_tag(pmustache, char *);

    void tag_handle(pmustache, char *, int, int, int);
    void tag_handle_variable(ptag_info);
    void tag_handle_no_escape(ptag_info);

    void tag_handle_sections(ptag_info);
    void tag_handle_sections_inverted(ptag_info);
    void tag_handle_sections_and_inverted_sections(ptag_info, bool);
    void tag_handle_comments(ptag_info);
    void tag_handle_partials(ptag_info);
    void tag_handle_delimiter(ptag_info);
    void tag_write_data(ptag_info, bool);
    //This will be used to clean up the tags which just means that it will remove the first character when the tag is not a variable
    void tag_clean(ptag_info);
    tag_end_data *tag_find_closing(ptag_info);

    void tag_set_error(char *);    
    bool tag_has_error(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TAGS_H */

