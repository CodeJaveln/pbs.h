#ifndef PBS_H
#define PBS_H

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ArgList {
    char **arr;
    size_t len;
    size_t cap;
} ArgList;

typedef struct PbsCmd {
    ArgList arg_list;
    bool valid;
} PbsCmd;

typedef struct Definition {
    const char *name;
    const char *value;
} Definition;

//Example:
//  compile_cmd_define(&cmd, DEFINE("FOO", 42));
#define DEFINE(def_name, def_value) \
    (Definition){ .name = def_name, .value = #def_value }

// CompileCmd *cmd, const char *sources...
//Example:
//  pbs_cc(&cmd, "src/main.c");
#define pbs_cc(cmd, ...) \
    pbs__cc(cmd, __VA_ARGS__, NULL)

// CompileCmd *cmd, Definition definitions...
#define pbs_define(cmd, ...) \
    pbs__define(cmd, __VA_ARGS__, (Definition){0})

// ArgList *arg_list, const char *args...
#define pbs_args_append(arg_list, ...) \
    pbs__args_append(arg_list, __VA_ARGS__, NULL)

void pbs__args_append(ArgList *arg_list, ...);

void pbs_args_va_append(ArgList *arg_list, va_list args);

void pbs__cc(PbsCmd *cmd, ...);

void pbs__define(PbsCmd *cmd, ...);

void pbs_output(PbsCmd *cmd, const char *outfile);

void pbs_run(PbsCmd *cmd);

#endif // PBS_H

#ifdef PBS_IMPLEMENTATION

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define da_append(xs, x) \
    do { \
        if (xs->len >= xs->cap) { \
            if (xs->cap == 0) xs->cap = 256; \
            else xs->cap *= 2; \
            xs->arr = realloc(xs->arr, xs->cap * sizeof(*xs->arr)); \
        } \
        xs->arr[xs->len++] = x; \
    } while (false)

void pbs__args_append(ArgList *arg_list, ...) {
    va_list args;
    va_start(args, arg_list);

    pbs_args_va_append(arg_list, args);

    va_end(args);
}

// TODO: make it good
void pbs_args_va_append(ArgList *arg_list, va_list args) {
    const char *str = va_arg(args, const char *);
    while (str) {
        char *d_str = strdup(str);
        da_append(arg_list, d_str);

        str = va_arg(args, const char *);
    }
}

static bool command_exists(const char *command) {
    pid_t pid = fork();
    if (pid == -1) {
        // TODO: Error
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        int fd = open("/dev/null", O_WRONLY);
        if (fd == -1) {
            // TODO: Error
            perror("open");
            exit(EXIT_FAILURE);
        }

        fd = dup2(fd, 1);
        if (fd == -1) {
            // TODO: Error
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        fd = dup2(1, 2);
        if (fd == -1) {
            // TODO: Error
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        execlp(command, command, "-v", (char *)NULL);

        // getting here would mean execlp failed!
        // execlp shouldn't return
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    pid_t w;
    int wstatus;
    do {
        w = waitpid(pid, &wstatus, 0);
        if (w == -1) {
            // TODO: Error
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    } while(!WIFEXITED(wstatus) && WIFSIGNALED(wstatus));

    return WEXITSTATUS(wstatus) == 0;
}

// How to find the C compiler:
//  Check if CC is defined (-D CC="cc") to override everything
//  Check $CC as an environment variable (getenv("CC"))
//  Try to find cc in $PATH
//  Try to find c99 int $PATH
//  If not: Scream at user they didn't provide a C compiler
static const char *get_cc() {
#ifdef CC
    return CC;
#else
    const char *cc = getenv("CC");
    if (cc) {
        return cc;
    }

    if (command_exists("cc")) {
        return "cc";
    }
    else if (command_exists("c99")) {
        return "c99";
    }

    fprintf(stderr, "no way to get a C compiler!\n");
    exit(EXIT_FAILURE);

    return NULL;
#endif
}

void pbs__cc(PbsCmd *cmd, ...) {
    const char *cc = get_cc();

    // TODO:
    pbs_args_append(&cmd->arg_list, cc);

    va_list va_sources;
    va_start(va_sources, cmd);

    pbs_args_va_append(&cmd->arg_list, va_sources);

    va_end(va_sources);

    cmd->valid = true;
}

void pbs__define(PbsCmd *cmd, ...) {
    va_list defs;
    va_start(defs, cmd);

    Definition def = va_arg(defs, Definition);
    while (def.name != NULL) {
        char *str = malloc(strlen(def.name) + strlen(def.value) + 2);
        sprintf(str, "%s=%s", def.name, def.value);
        pbs_args_append(&cmd->arg_list, "-D", str);

        def = va_arg(defs, Definition);
    }

    va_end(defs);
    // check cmd->valid == true
}

void pbs_output(PbsCmd *cmd, const char *outfile) {
    pbs_args_append(&cmd->arg_list, "-o", outfile);
}

void pbs_run(PbsCmd *cmd) {
    ArgList *arg_list = &cmd->arg_list;
    da_append(arg_list, NULL);

    pid_t pid = fork();
    if (pid == -1) {
        // TODO: Error
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execvp(cmd->arg_list.arr[0], cmd->arg_list.arr);

        // getting here would mean execlp failed!
        // execlp shouldn't return
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    pid_t w;
    int wstatus;
    do {
        w = waitpid(pid, &wstatus, 0);
        if (w == -1) {
            // TODO: Error
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    } while(!WIFEXITED(wstatus) && WIFSIGNALED(wstatus));

    // check cmd->valid == true
}

#endif // PBS_IMPLEMENTATION
