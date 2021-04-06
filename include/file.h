#ifndef __FILE_H
#define __FILE_H

#include <sys/stat.h>

char * get_proc_comm(const char *pid, char *comm);
char * get_proc_user(const char *pid, char *user);
char * get_type(struct stat file_stat, char *type);
char * get_node(struct stat file_stat, char *type);
void get_permission(char *path, char* permission);

#endif