#define PBS_IMPLEMENTATION
#include "pbs.h"

int main() {
    PbsCmd cmd = {0};

    pbs_cc(&cmd, "src/main.c");
    pbs_output(&cmd, "bin/game");

    pbs_run(&cmd);

    for (size_t i = 0; cmd.arg_list.arr[i] != NULL; i++) {
        printf("%s\n", cmd.arg_list.arr[i]);
    }

    return 0;
}
