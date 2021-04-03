#include <stdbool.h>
#include "mode.h"

unsigned char mode = 0b00000000;

void set_mode_on(const char mode_char) {
    enum MODE_MASK mask;

    switch (mode_char) {
        case 'c':
            mask = CMD_MODE;
            break;
        case 't':
            mask = TYPE_MODE;
            break;
        case 'f':
            mask = FILE_MODE;
            break;
        default:
            mask = 0b00000000;
            break;
    }

    mode |= mask;
}


bool is_mode_on(enum MODE_MASK mask)
{
    return (mode & mask) > 0;
}