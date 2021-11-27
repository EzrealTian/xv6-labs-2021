/*
 * Write a simple version of the UNIX xargs program: 
 * read lines from the standard input and run a command for each line, 
 * supplying the line as arguments to the command. Your solution should be in the file user/xargs.c.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define FDSTDIN 0 
#define FDSTDOUT 1

int main(int argc,char* argv[]) {
    char* cmd[MAXARG];
    int arg_index;
    for (arg_index = 0; arg_index < argc - 1; arg_index++)
        cmd[arg_index] = argv[arg_index + 1];

    char buf;  
    cmd[arg_index] = malloc(512);

    int char_index = 0;
    while(read(FDSTDIN, &buf, 1)) { 
        switch (buf) {
        case '\n':
            if (fork() == 0) 
                exec(cmd[0], cmd);
            wait(0);
            for (int i = argc; i <= arg_index; i++)
                free(cmd[i]);
            arg_index = argc - 1;
            char_index = 0;
            cmd[arg_index] = malloc(512);
            break;
        case ' ':
            if (arg_index < MAXARG) {
                arg_index++;
                char_index = 0;
                cmd[arg_index] = malloc(512);
            }
            break;
        default:
            cmd[arg_index][char_index] = buf;
            char_index++;
            break;
        }
    }	   
    exit(0);
}