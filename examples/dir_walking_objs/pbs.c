#define PBS_IMPLEMENTATION
#include "pbs.h"

int walk_func(PbsDirEntry *dirent, void *ud) {
    PbsCmd cmd = {0};
    pbs_cc(&cmd);
}

int main(int argc, char *argv[]) {
    pbs_rebuild(argc, argv, "pbs.c", "pbs.h");

    pbs_mkdir("build/");

    pbs_walkdir("src/", walk_func, NULL);
}
