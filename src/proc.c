#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "proc.h"
#include "filter.h"

#define MAX_STRLEN (1024)

void list_proc_content()
{
    int length;
    int *pid_arr = get_sorted_pid_arr(&length);
    printf("length: %d\n", length);

    for (int i = 0; i < length; i++) {
        list_pid_dir_content(pid_arr[i]);
    }

    // free pid_arr
}

/* cwd -> root -> exe -> mem -> fd([0-9]+[rwu]) */

// directory name is pid
void list_pid_dir_content(const int pid)
{   
    char pid_string[MAX_STRLEN] = "", 
         comm[MAX_STRLEN] = "";

    sprintf(pid_string, "%d", pid);
    get_proc_comm(pid_string, comm);
    
    printf("pid: %s, comm: %s\n", pid_string, comm);

    char dir_path[MAX_STRLEN] = "/proc/";
    strcat(dir_path, pid_string);
    
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
            printf("get_sorted_pid_arr: %s\n", entry->d_name);
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
