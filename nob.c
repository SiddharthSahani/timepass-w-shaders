
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"


int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = { 0 };

    cmd_append(&cmd, "cc", "-Wall", "-Werror");
    cmd_append(&cmd, "-Iexternal");
    cmd_append(&cmd, "-o", "vornoi", "src/vornoi.c");
    cmd_append(&cmd, "-Lexternal/raylib", "-lraylib", "-lgdi32", "-lwinmm");
    if (!cmd_run_sync_and_reset(&cmd)) {
        return 1;
    }

    cmd_append(&cmd, "./vornoi", "128", "50");
    if (!cmd_run_sync_and_reset(&cmd)) {
        return 1;
    }

    return 0;
}
