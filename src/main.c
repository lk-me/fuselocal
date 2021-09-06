#include <stdio.h>
#include <unistd.h>

#include "fuse_local.h"

int main(int argc, char *argv[])
{
    char **fuse_argv = NULL;
    fuse_local_init(argc, argv);
    return 0;
}
