#include "../inc/util.h"

/**
 * Escape characters (change them) &, ", ', < and >
 *
 * @param string Text
 * @return String with text replaced
 */
char *text_escape(const char *string) {
    if (string != NULL) {
        int len = strlen(string);

        int escape_size = len * 2;
        int escaped_index = 0;
        char *escaped_text = calloc(escape_size, sizeof (char));

        int index = 0;

        int add = 0;
        const char *to_add = NULL;

        while (string[index] != '\0') {

            switch (string[index]) {
                case '&':
                    to_add = "&amp;";
                    add = 5;
                    break;
                case '\"':
                    to_add = "&quot;";
                    add = 6;
                    break;
                case '\'':
                    to_add = "&apos;";
                    add = 6;
                    break;
                case '<':
                    to_add = "&lt;";
                    add = 4;
                    break;
                case '>':
                    to_add = "&gt;";
                    add = 4;
                    break;
                default:
                    to_add = &(string[index]);
                    add = 1;
            }

            if (add > 1) {
                if ((escaped_index + add) > escape_size - 1) {
                    escape_size += add * 2;

                    char *tmp_buffer = realloc(escaped_text, escape_size * sizeof (char));
                    if (tmp_buffer != NULL) {
                        escaped_text = tmp_buffer;
                    }
                    else {
                        perror("Unable to allocate memory for tmp_buffer\n");
                        exit(-1);
                    }
                }

                strcat(escaped_text, to_add);
                escaped_index += add;
            }
            else {
                escaped_text[escaped_index++] = *to_add;
            }

            index++;
        }

        return escaped_text;
    }

    return NULL;
}
