#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>

#include "proc.h"
#include "filter.h"

#define MAX_STRLEN (1024)

void list_proc_content()
{
    int length;
    int *pid_arr = get_sorted_pid_arr(&length);

    for (int i = 0; i < length; i++) {
        list_pid_dir_content(pid_arr[i]);
    }

    // free pid_arr
    free(pid_arr);
}

/* cwd -> root -> exe -> mem -> fd([0-9]+[rwu]) */

// directory name is pid
void list_pid_dir_content(const int pid)
{   
    char pid_string[MAX_STRLEN] = "",
         comm[MAX_STRLEN] = "",
         owner[MAX_STRLEN] = "",
         dir_path[MAX_STRLEN] = "/proc/";
    
    strcat(dir_path, pid_string);

    /* int to string */
    sprintf(pid_string, "%d", pid);
    get_proc_comm(pid_string, comm);
    get_owner(dir_path, owner);

    printf("pid: %s, command: %s, owner: %s\n", pid_string, comm, owner);
    
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

int * get_sorted_pid_arr(int *length)
{
    *length = pid_dir_count();
    int *pid_arr = (int *)malloc(sizeof(int) * (*length));

    DIR *proc_dir;
    struct dirent *entry;
    proc_dir = opendir("/proc");

    int i = 0;
    while ((entry = readdir(proc_dir)) != NULL) {
        if (is_pure_num(entry->d_name)) {
            pid_arr[i] = atoi(entry->d_name);
            i++;
        }
    }

    closedir(proc_dir);
    qsort(pid_arr, *length, sizeof(int), _compare);

    return pid_arr;
}

int pid_dir_count()
{
    struct dirent *entry;
    DIR *proc_dir = opendir("/proc");
    int count = 0;
    while ((entry = readdir(proc_dir)) != NULL) {
        if (is_pure_num(entry->d_name)) {
            count++;
        }
    }

    closedir(proc_dir);

    return count;
}

int _compare(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}


void get_owner(char *dir_path, char *owner)
{
    struct stat dir_stat;
    struct passwd *pwd;

    if (stat(dir_path, &dir_stat) == -1) {
        perror("stat");
        exit(-1);
    };

    pwd = getpwuid(dir_stat.st_uid);

    strcpy(owner, pwd->pw_name);
}