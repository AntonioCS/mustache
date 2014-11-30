/* 
 * File:   text.h
 * Author: antoniocs
 *
 * Created on 23 de Novembro de 2014, 14:05
 */
#include "mustache.h"

#ifndef TEXT_H
#define	TEXT_H

#ifdef	__cplusplus
extern "C" {
#endif

void text_set(pmustache, char *);
char text_get_char(pmustache);
char text_get_char_pos(pmustache, int);

void text_set_size(pmustache);
int text_get_size(pmustache);

#ifdef	__cplusplus
}
#endif

#endif	/* TEXT_H */

