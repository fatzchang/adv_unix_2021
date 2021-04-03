#ifndef __PROC_H
#define __PROC_H

void list_proc_content(const char *pid);
char * get_proc_comm(const char *pid, char *comm);

#endif