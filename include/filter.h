#ifndef __FILTER_H
#define __FILTER_H

#define NUM_REGEX_STRING ("^[0-9]+$")
#define TYPE_REGEX_STRING ("^REG$|^CHR$|^DIR$|^FIFO$|^SOCK$|^unknown$")
#include <stdbool.h>
#include "mode.h"

bool is_pure_num(char *target);
bool is_valid_type(const char *type);
bool match_filter(enum MODE_MASK filter_mode, const char *val);

#endif