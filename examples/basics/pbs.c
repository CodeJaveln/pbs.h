#define PBS_IMPLEMENTATION
#include "pbs.h"

int main(int argc, char *argv[]) {
    // Simple rebuild:
    // checks __FILE__ and compiles to argv[0].
    pbs_go_rebuild(argc, argv);

    PbsCmd cmd = {0};

    pbs_cc(&cmd, "src/main.c");
    pbs_output(&cmd, "bin/game");
    // Can define macro values easily if needed.
    //pbs_define(&cmd, DEFINE("FOO", "bar"));

    pbs_run(&cmd);

    //for (size_t i = 0; cmd.arg_list.arr[i] != NULL; i++) {
    //    printf("%s\n", cmd.arg_list.arr[i]);
    //}

    return EXIT_SUCCESS;
}
