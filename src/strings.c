#include "../inc/strings.h"

pmstash_str mstash_str_init() {
    pmstash_str newstr = malloc(sizeof (mstash_str));

    if (newstr != NULL) {
        newstr->str = calloc(MSTASH_STRING_INIT_SIZE, sizeof (char));
        if (newstr->str == NULL) {
            perror("Unable to allocate space for string");
            exit(-1);
        }

        newstr->increase_by = newstr->size = MSTASH_STRING_INIT_SIZE;
        newstr->length = 0;

        return newstr;
    }

    return NULL;
}

void mstash_str_add_char(pmstash_str string, int c) {

    if (mstash_str_is_resize_needed(string, 1)) {
        mstash_str_resize(string);
    }

    string->str[string->length++] = c;
}

void mstash_str_add_str(pmstash_str s, char *str) {
    int len = strlen(str);
    mstash_str_add_str_num(s, str, len);
}

void mstash_str_add_str_num(pmstash_str s, char *str, int length) {
    while (mstash_str_is_resize_needed(s, length)) {
        mstash_str_resize(s);
    }

    memcpy(s->str + s->length, str, length);
    s->length += length;
}

bool mstash_str_is_resize_needed(pmstash_str s, int len_to_add) {
    return (s->length + len_to_add) >= s->size;
}

void mstash_str_resize(pmstash_str string) {
    char *tmp_buffer = realloc(string->str, (string->size += string->increase_by));

    if (tmp_buffer) {
        string->str = tmp_buffer;
        memset(string->str + string->length, 0, string->size - string->length);
    } else {
        perror("Unable to reallocate memory");
        exit(-1);
    }
}

void mstash_str_destroy(pmstash_str s) {
    free(s->str);
    free(s);
}

void mstash_str_clear(pmstash_str s) {
    s->size = 0;
    s->length = 0;
    s->increase_by = MSTASH_STRING_INIT_SIZE;

    mstash_str_resize(s);
}

void mstash_str_from_file(pmstash_str s, const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp != NULL) {
        char buffer[MSTASH_READ_FILE_BUFFER_SIZE] = {'\0'};
        int buf_size = MSTASH_READ_FILE_BUFFER_SIZE - 1;
        int inc_by = s->increase_by;
        size_t read;

        s->increase_by = MSTASH_READ_FILE_BUFFER_SIZE;

        do {
            read = fread(buffer, 1, buf_size, fp);
            mstash_str_add_str_num(s, buffer, read);                       
        } while (read == buf_size);

        s->increase_by = inc_by;
        fclose(fp);
    }
}
