/* 
 * File:   text_parsed.h
 * Author: antoniocs
 *
 * Created on 23 de Novembro de 2014, 14:06
 */
#include "mustache.h"

#ifndef TEXT_PARSED_H
#define	TEXT_PARSED_H

#ifdef	__cplusplus
extern "C" {
#endif

bool text_parsed_init(pmustache);
void text_parsed_add_char(pmustache, char *);
void text_parsed_add_string(pmustache m, char *);

#ifdef	__cplusplus
}
#endif

#endif	/* TEXT_PARSED_H */

