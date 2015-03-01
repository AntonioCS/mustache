/* 
 * File:   position_file.h
 * Author: antoniocs
 *
 * Created on 8 de Fevereiro de 2015, 19:29
 */

#include "mustache.h"

#ifndef POSITION_FILE_H
#define	POSITION_FILE_H

#ifdef	__cplusplus
extern "C" {
#endif

size_t pos_get_file(pmustache);
void pos_inc_file(pmustache);
void pos_dec_file(pmustache);
//size_t pos_get_inc(pmustache);
void pos_set_file(pmustache, int);



#ifdef	__cplusplus
}
#endif

#endif	/* POSITION_FILE_H */

