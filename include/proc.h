#ifndef __PROC_H
#define __PROC_H

void list_proc_content();
void list_pid_dir_content(const int pid);
int * get_sorted_pid_arr(int *length);
int pid_dir_count();
int _compare(const void *a, const void *b);
void print_file(const char *command, const char *pid_string, const char *user, const char *filename);

#endif