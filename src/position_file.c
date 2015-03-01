#include "../inc/position_file.h"

/**
 * Allocate size of fpos_p 
 * @param m
 */
static void check_for_pos_obj(pmustache m) {
    /**          
     * @todo Add code somewhere to free this when not needed anymore
     */
    if (m->pos_obj == NULL) {
        m->pos_obj = malloc(sizeof (fpos_t));
    }
    
    fgetpos(m->mfile, m->pos_obj);
}

size_t pos_get_file(pmustache m) {
    check_for_pos_obj(m);

    return (size_t) m->pos_obj->__pos; //long int
}

void pos_inc_file(pmustache m) {
    check_for_pos_obj(m);

    /** @todo check if it doesn't go over file limit */
    pos_set_file(m, m->pos_obj->__pos++);
}

void pos_dec_file(pmustache m) {
    check_for_pos_obj(m);

    /** @todo check if it doesn't go below 0 */
    pos_set_file(m, m->pos_obj->__pos--);
}

void pos_set_file(pmustache m, int p) {
    check_for_pos_obj(m);

    /** @todo check if it doesn't go over file limit */
    m->pos_obj->__pos = p;
    fsetpos(m->mfile, m->pos_obj);
}

