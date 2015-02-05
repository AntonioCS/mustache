/* 
 * File:   position.h
 * Author: antoniocs
 *
 * Created on 23 de Novembro de 2014, 14:14
 */

#include "mustache.h"

#ifndef POSITION_H
#define	POSITION_H

#ifdef	__cplusplus
extern "C" {
#endif
//http://blackfin.uclinux.org/doku.php?id=toolchain:elf_visibility
size_t pos_get(pmustache);
void pos_inc(pmustache);
void pos_dec(pmustache);
size_t pos_get_inc(pmustache);
void pos_set(pmustache, int);

#ifdef	__cplusplus
}
#endif

#endif	/* POSITION_H */

