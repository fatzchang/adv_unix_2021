#ifndef __MODE_H
#define __MODE_H

#include <stdbool.h>

enum MODE_MASK {
    CMD_MODE    = 1,
    TYPE_MODE   = 2,
    FILE_MODE   = 4
};

void set_mode_on(const char mode_char);
bool is_mode_on(enum MODE_MASK mask);

#endif