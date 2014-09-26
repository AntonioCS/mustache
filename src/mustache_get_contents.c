/*
 * File:   mustache_get_contents.c
 * Author: AntonioCS
 *
 * Created on 25 de Setembro de 2014, 22:36
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mustache_get_contents.h"


char *mustache_get_contents(char *file_path) {
    FILE *fp = fopen(file_path, "r");

    if (fp) {
        char data[MUSTACHE_READFILE_BUFFER] = { 0 };
        char *text = malloc(sizeof(char) * MUSTACHE_READFILE_BUFFER);
        char *text_tmp = NULL;
        int text_inc = 0;
        int text_jump = 0;
        int data_read = 0 ;

        while (fgets(data, MUSTACHE_READFILE_BUFFER, fp) != NULL) {
            data_read += strlen(data);
            strcat(text, data);




            /*
            printf("Reading: %s\n", data);
            text_jump = text_inc * MUSTACHE_READFILE_BUFFER;

            if (text_inc != 0) {
                text_tmp = realloc(text, sizeof(char) * (++text_inc * MUSTACHE_READFILE_BUFFER));

                if (text_tmp != NULL) {
                    text = text_tmp;
                    //memset(text + text_jump, 0, MUSTACHE_READFILE_BUFFER);
                }
                else {
                    printf("Error allocating memory\n");
                    exit(-1);
                }
            }
            else {
                text_inc++;
            }

            memcpy(text + text_jump, data, MUSTACHE_READFILE_BUFFER);

            memset(data, 0, MUSTACHE_READFILE_BUFFER);
             */
        }


        fclose(fp);

        return text;
    }

    return NULL;
}
