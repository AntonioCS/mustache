/* 
 * File:   text_file.h
 * Author: antoniocs
 *
 * Created on 3 de Fevereiro de 2015, 8:27
 */
#include "mustache.h"

#ifndef TEXT_FILE_H
#define	TEXT_FILE_H

#ifdef	__cplusplus
extern "C" {
#endif

void text_file_set(pmustache, char *);
char text_file_get_char(pmustache);
char text_file_get_char_pos(pmustache, int);

void text_file_set_size(pmustache, long int);
int text_file_get_size(pmustache);



#ifdef	__cplusplus
}
#endif

#endif	/* TEXT_FILE_H */

