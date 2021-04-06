#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include "hw1.h"
#include "proc.h"
#include "filter.h"
#include "file.h"
#include "display.h"


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
void list_pid_dir_content(const int pid)
{   
    char pid_string[MAX_STRLEN] = "",
         comm[MAX_STRLEN] = "",
         user[MAX_STRLEN] = "",
         dir_path[MAX_STRLEN] = "/proc/",
         target_path[MAX_STRLEN] = "";
    
    /* int to string */
    sprintf(pid_string, "%d", pid);
    strcat(dir_path, pid_string);
    get_proc_comm(pid_string, comm);
    get_proc_user(pid_string, user);

    
    char * order[3] = {"cwd", "root", "exe"};
    for (int i = 0; i < 3; i++) {
        memset(target_path, 0, sizeof(char) * MAX_STRLEN);
        strcat(target_path, dir_path);
        strcat(target_path, order[i]);
        // debugging   
        print_file(comm, pid_string, user, order[i]);    
    }
    

    /* mem */
    list_mem_maps(comm, pid_string, user);

    /* fd */
    list_fd(comm, pid_string, user);

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
