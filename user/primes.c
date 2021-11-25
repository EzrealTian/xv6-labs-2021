/*
 * Write a concurrent version of prime sieve using pipes. 
 * This idea is due to Doug McIlroy, inventor of Unix pipes. 
 * The picture halfway down this page and the surrounding text explain how to do it. 
 * Your solution should be in the file user/primes.c.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 35

void pipeline(int in, int out);
int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    for (int i = 2; i <= MAX_NUM; i++) 
        write(p[1], &i, sizeof(i));
    close(p[1]);
    pipeline(p[0], p[1]);
    exit(0);
}

void pipeline(int read_fd, int write_fd) {
    int prime, num;
    if (!read(read_fd, &prime, sizeof(prime)))
        exit(0);
    printf("prime %d\n", prime);

    if (fork() == 0) {
        int p[2];
        pipe(p);
        while (read(read_fd, &num, sizeof(num))) {
            if (num % prime != 0)
                write(p[1], &num, sizeof(num));
        }
        close(p[1]);
        close(read_fd);
        pipeline(p[0], p[1]);
    } else {
        wait(0);
    }
    exit(0);
}