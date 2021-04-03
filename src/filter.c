#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "filter.h"

bool is_pure_num(char *target)
{   
    static int initialized = 0;
    static regex_t compiled_regex;
   
    if (!initialized) {
        if (!regcomp(&compiled_regex, NUM_REGEX_STRING, REG_EXTENDED | REG_NEWLINE) == 0) {
            printf("compile failed\n");
            exit(-1);
        } 
    
        initialized = 1;
    }

    int status = regexec(&compiled_regex, target, 0, NULL, 0);
    
    return status == 0;
}

bool is_valid_type(const char *type) {
    regex_t compiled_regex;
    if (!regcomp(&compiled_regex, TYPE_REGEX_STRING, REG_EXTENDED | REG_NEWLINE) == 0) {
        printf("compile failed\n");
        exit(-1);
    }

    int status = regexec(&compiled_regex, type, 0, NULL, 0);
    
    return status == 0;
}