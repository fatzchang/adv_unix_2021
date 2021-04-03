#include "proc.h"
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_STRLEN (1024)

void list_proc_content(const char *pid)
{   
    char comm[MAX_STRLEN] = "";
    get_proc_comm(pid, comm);
    
    printf("pid: %s, comm: %s\n", pid, comm);

    char dir_path[MAX_STRLEN] = "/proc/";
    strcat(dir_path, pid);
    
    DIR *pid_dir;
    struct dirent *entry;
    pid_dir = opendir(dir_path);
}

char * get_proc_comm(const char *pid, char *comm)
{
    char comm_path[MAX_STRLEN] ="";
    strcat(comm_path, "/proc/");
    strcat(comm_path, pid);
    strcat(comm_path, "/comm");


    FILE *fp;
    fp = fopen(comm_path, "r");
    fscanf(fp, "%s", comm);

    return comm;
}