/*
 * Write a simple version of the UNIX find program: find all the files in a directory tree with a specific name. 
 * Your solution should be in the file user/find.c.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void fmtname(char* path, char* file) {
    char *p;
    // Find first character after last slash.
    for(p = path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    if (!strcmp(p, file))
        printf("%s\n", path);
}

void find(char* path, char* file) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: %s is not a directory\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_FILE:
            fmtname(path, file);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                // don't recurse into . and ..
                if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                switch (st.type) {
                    case T_FILE:
                        fmtname(buf, file);
                        break;
                    case T_DIR:
                        find(buf, file);
                        break;
                }
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "find: parameter count error!\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}