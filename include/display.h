#ifndef __DISPLAY_H
#define __DISPLAY_H

void print_file(const char *command, const char *pid_string, const char *user, const char *path);
void list_mem_maps(const char *command, const char *pid_string, const char *user);
void list_fd(const char *command, const char *pid_string, const char *user);
void format_printer(
    const char *command, 
    const char *pid, 
    const char *user, 
    const char *fd, 
    const char *type, 
    const char *node, 
    const char *fd_real_path
);

#endif