/* 
 * File:   strings.h
 * Author: antoniocs
 *
 * Created on 23 de Novembro de 2014, 17:27
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef STRINGS_H
#define	STRINGS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MSTASH_STRING_INIT_SIZE 15
#define MSTASH_READ_FILE_BUFFER_SIZE 1024

typedef struct mstash_str {
    char *str;

    int size;
    int length;

    int increase_by; //will have the default of 15 but can be customized per instance
} mstash_str, *pmstash_str;

pmstash_str mstash_str_init(void);
void mstash_str_add_char(pmstash_str, int);
void mstash_str_add_str(pmstash_str, char *);
void mstash_str_add_str_num(pmstash_str, char *, int);
void mstash_str_resize(pmstash_str);
void mstash_str_destroy(pmstash_str);
void mstash_str_clear(pmstash_str);
bool mstash_str_is_resize_needed(pmstash_str, int);
void mstash_str_from_file(pmstash_str, const char *);


#ifdef	__cplusplus
}
#endif

#endif	/* STRINGS_H */

