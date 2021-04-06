#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "filter.h"
#include "mode.h"

extern char *cmd_reg_string;
extern char *file_reg_string;
extern char *type;

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
    if (regcomp(&compiled_regex, TYPE_REGEX_STRING, REG_EXTENDED | REG_NEWLINE) != 0) {
        printf("compile failed\n");
        exit(-1);
    }

    int status = regexec(&compiled_regex, type, 0, NULL, 0);
    
    return status == 0;
}

bool match_filter(enum MODE_MASK filter_mode, const char *val)
{
    regex_t compiled_regex;
    int compile_status, match_status;

    switch (filter_mode) {
        case CMD_MODE:
            compile_status = regcomp(&compiled_regex, cmd_reg_string, REG_EXTENDED | REG_NEWLINE);
            break;
        case TYPE_MODE:
            compile_status = regcomp(&compiled_regex, type, REG_EXTENDED | REG_NEWLINE);
            break;
        case FILE_MODE:
            compile_status = regcomp(&compiled_regex, file_reg_string, REG_EXTENDED | REG_NEWLINE);
            break;
        default:
            break;
    }
    
    if (compile_status != 0) {
        perror("filter regex compile failed");
        exit(-1);
    }
    match_status = regexec(&compiled_regex, val, 0, NULL, 0);

    return match_status == 0;
}