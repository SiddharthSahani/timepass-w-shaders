
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"


int buildExample(const char* example) {
    bool extPresent = sv_end_with(sv_from_cstr(example), ".c");

    char inputFile[256] = { 0 };
    if (!extPresent) {
        sprintf(inputFile, "src/%s.c", example);
    } else {
        sprintf(inputFile, "src/%s", example);
    }
    char outputFile[256] = { 0 };
    sprintf(outputFile, "build/%s", example);

    Cmd cmd = { 0 };
    cmd_append(&cmd, "cc", "-Wall", "-Werror");
    cmd_append(&cmd, "-Iexternal");
    cmd_append(&cmd, "-o", outputFile, inputFile);
    cmd_append(&cmd, "-Lexternal/raylib", "-lraylib", "-lgdi32", "-lwinmm");

    return cmd_run_sync(cmd);
}


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./nob <example or 'all'>\n");
        return 1;
    }

    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = { 0 };

    if (!mkdir_if_not_exists("build")) {
        return 1;
    }

    if (strcmp(argv[1], "all") != 0) {
        return !buildExample(argv[1]);
    }

    // building all examples
    File_Paths examples = { 0 };
    read_entire_dir("src", &examples);

    for (size_t i = 0; i < examples.count; i++) {
        const char* example = examples.items[i];
        if (strcmp(example, ".") == 0 || strcmp(example, "..") == 0) {
            continue;
        }
        if (!buildExample(example)) {
            return 1;
        }
    }
}
