#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include "hw1.h"
#include "display.h"

char * get_proc_comm(const char *pid, char *comm)
{
    char comm_path[MAX_STRLEN] ="";
    strcat(comm_path, "/proc/");
    strcat(comm_path, pid);
    strcat(comm_path, "/comm");

    FILE *fp;
    fp = fopen(comm_path, "r");
    fscanf(fp, "%s", comm);
    fclose(fp);

    return comm;
}


char * get_proc_user(const char *pid, char *user)
{
    char path[MAX_STRLEN] = "/proc/";
    strcat(path, pid);

    static struct stat file_stat;
        
    if (stat(path, &file_stat) == -1) {
        perror("pid dir stat");
        exit(-1);
    };

    struct passwd *pwd = getpwuid(file_stat.st_uid);

    strcpy(user, pwd->pw_name);

    return user;
}

char * get_type(struct stat file_stat, char *type)
{
    mode_t m = file_stat.st_mode;

    if (m & S_IFREG) {
        strcpy(type, "REG");
    } else if (m & S_IFDIR) {
        strcpy(type, "DIR");
    } else if (m & S_IFCHR) {
        strcpy(type, "CHR");
    } else if (m & S_IFIFO) {
        strcpy(type, "FIFO");
    } else if (m & S_IFSOCK) {
        strcpy(type, "SOCK");
    } else {
        strcpy(type, "unknown");
    }

    return type;
}

char * get_node(struct stat file_stat, char *type) 
{
    sprintf(type, "%d", (int)file_stat.st_ino);

    return type;
}

void get_permission(char *path, char* permission)
{
    struct stat file_lstat;
    if (lstat(path, &file_lstat) == -1) {
        perror("lstat permission");
        exit(-1);
    }

    mode_t m = file_lstat.st_mode;
    if ((m & S_IRUSR) && (m & S_IWUSR)) {
        permission[0] = 'u';
    } else if (m & S_IRUSR) {
        permission[0] = 'r';
    } else if (m & S_IWUSR) {
        permission[0] = 'w';
    }
}