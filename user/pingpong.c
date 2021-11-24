/*
 * Write a program that uses UNIX system calls to ''ping-pong'' a byte between two processes over a pair of pipes, 
 * one for each direction. 
 * The parent should send a byte to the child; 
 * the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit; 
 * the parent should read the byte from the child, print "<pid>: received pong", and exit. 
 * Your solution should be in the file user/pingpong.c.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    int p[2];
    char buffer[1];
    pipe(p);

    if (fork() == 0) {
        read(p[0], buffer, sizeof(buffer));
        printf("%d: received ping\n", getpid());
        write(p[1], buffer, sizeof(buffer));
        exit(0);
    } else {
        write(p[1], buffer, sizeof(buffer));
        wait(0);
        read(p[0], buffer, sizeof(buffer));
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}