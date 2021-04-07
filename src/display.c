#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#include "hw1.h"
#include "file.h"
#include "display.h"
#include "filter.h"
#include "mode.h"

extern errno;

// for cwd root exe
void print_file(const char *command, const char *pid_string, const char *user, const char *filename)
{
    char path[MAX_STRLEN] = "/proc/",
         type[MAX_STRLEN] = "",
         node[MAX_STRLEN] = "",
         name[MAX_STRLEN] = "";

    strcat(path, pid_string);
    strcat(path, "/");
    strcat(path, filename);

    struct stat file_stat;

    // check permission
    if (stat(path, &file_stat) == -1) {
        if (errno == ENOENT) {
            return;
        }
        
        strcpy(name, path);
        strcat(name, " (readlink: Permission denied)");
        strcpy(type, "unknown");
    } else {
        get_type(file_stat, type);
        get_node(file_stat, node);
        realpath(path, name);
    }

    format_printer(command, pid_string, user, filename, type, node, name); 
}

// for maps
void list_mem_maps(const char *command, const char *pid_string, const char *user)
{
    char path[MAX_STRLEN] = "/proc/";
    strcat(path, pid_string);
    strcat(path, "/maps");

    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        return;
    }

    char buf[MAX_STRLEN];

    char prev_node[MAX_STRLEN] = "";
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        char node[MAX_STRLEN] = "";
        char name[MAX_STRLEN] = "";
        char stat[MAX_STRLEN] = "";

        sscanf(buf, "%*s %*s %*s %*s %s %s %s", node, name, stat);

        if (strcmp(node, "0") == 0) {
            continue;
        }

        // ignore duplicate
        if (strcmp(node, prev_node) == 0) {
            continue;
        }

        memset(prev_node, 0, sizeof(char) * MAX_STRLEN);
        strcpy(prev_node, node);

        if (strcmp(stat, "(deleted)") == 0) {
            strcat(name, " (deleted)");
            format_printer(command, pid_string, user, "del", "unknown", node, name); 
        } else {
            format_printer(command, pid_string, user, "mem", "REG", node, name); 
        }


    }
    
    fclose(fp);   
}

void list_fd(const char *command, const char *pid_string, const char *user)
{
    char path[MAX_STRLEN] = "/proc/";
    DIR *fd_dir;
    struct dirent *entry;
    strcat(path, pid_string);
    strcat(path, "/fd");

    if (!(fd_dir = opendir(path))) {
        strcat(path, " (opendir: Permission denied)");
        format_printer(command, pid_string, user, "NOFD", "", "", path);
        return;
    } 

    while ((entry = readdir(fd_dir)) != NULL) {
        if (!is_pure_num(entry->d_name)) {
            // ignore . and ..
            continue;
        }
        
        // read file stat
        char fd_path[MAX_STRLEN] = "", 
             fd_real_path[MAX_STRLEN] = "", 
             type[MAX_STRLEN] = "",
             node[MAX_STRLEN] = "",
             fd[MAX_STRLEN] = "",
             permission[2] = "";

        strcpy(fd_path, path);
        strcat(fd_path, "/");
        strcat(fd_path, entry->d_name);

        get_permission(fd_path, permission);

        struct stat fd_stat;

        if (stat(fd_path, &fd_stat) == -1) {
            perror("fd stat");
            exit(-1);
        }
        get_type(fd_stat, type);
        get_node(fd_stat, node);
        readlink(fd_path, fd_real_path, MAX_STRLEN);

        strcpy(fd, entry->d_name);
        strcat(fd, permission);


        if (strstr(fd_real_path, "(deleted)") != NULL) {
            format_printer(command, pid_string, user, fd, "unknown", node, fd_real_path);
        } else {
            format_printer(command, pid_string, user, fd, type, node, fd_real_path);
        }
    
    }


    closedir(fd_dir);
}

void format_printer(
    const char *command, 
    const char *pid, 
    const char *user, 
    const char *fd, 
    const char *type, 
    const char *node, 
    const char *fd_real_path
) {
    if (strcmp(command, "COMMAND") != 0) {
        if (is_mode_on(CMD_MODE) && !match_filter(CMD_MODE, command)) {
            return;
        }

        if (is_mode_on(TYPE_MODE) && !match_filter(TYPE_MODE, type)) {
            return;
        }

        if (is_mode_on(FILE_MODE) && !match_filter(FILE_MODE, fd_real_path)) {
            return;
        }
    }

    printf("%-5s %-5s %-5s %-5s %-8s %-10s %-5s\n", command, pid, user, fd, type, node, fd_real_path);
}


