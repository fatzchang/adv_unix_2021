#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "hw1.h"
#include "filter.h"
#include "proc.h"
#include "mode.h"

/* filter srings, has to be initialize */
char *cmd_reg_string, *file_reg_string, *type;

int main(int argc, char *argv[])
{
    /* parse args */
    if (parse_args(argc, argv) != 0) {
        return -1;
    }

    list_proc_content();
    return 0;
}

int parse_args(int argc, char *argv[])
{
    int opt, rtn = 0;
    while((opt = getopt(argc, argv, "c:t:f:")) != -1) {
        set_mode_on(opt);

        switch (opt) {
            case 'c':
                cmd_reg_string = optarg;
                break;
            case 't':
                type = optarg;
                rtn = is_valid_type(type) ? 0 : -1;
                if (rtn == -1) {
                    printf("Invalid TYPE option.\n");
                }
                break;
            case 'f':
                file_reg_string = optarg;
                break;
            default:
                // TODO: default, missing argument ??
                rtn = -1;
                break;
        }
    }

    return rtn;
}