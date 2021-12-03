#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if(fork() == 0) {
        char buf[512];
        close(p[1]);
        while (read(p[0], buf, 512) != 0) {
            printf("%s\n", buf);
        }
        printf("read done\n");
        close(p[0]);
        exit(0);
    } else {
        close(p[0]);
        for (int i = 0; i < 10; i++) {
            write(p[1], "hello world\n", 12);
            //printf("%d write done!\n", i);
        }
        //close(p[1]);
        wait(0);
    }
    printf("done\n");
    exit(0);
}