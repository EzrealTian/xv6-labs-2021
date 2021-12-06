#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
    uint32 t = 1 << 31;
    printf("%d\n", t);
    exit(0);
}