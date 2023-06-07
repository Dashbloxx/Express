#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "conf.h"

int parse_conf(char* line, char* key, char* value)
{
    char* equal_sign = strchr(line, '=');

    if (equal_sign == NULL) {
        return -1;
    }

    unsigned int key_length = equal_sign - line;
    strncpy(key, line, key_length);
    key[key_length] = '\0';

    unsigned int value_length = strlen(equal_sign + 1);
    strncpy(value, equal_sign + 1, value_length);
    value[value_length] = '\0';

    return 0;
}