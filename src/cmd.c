#define _POSIX_C_SOURCE 200809L

#include "cmd.h"
#include "parser.h"
#include "pipe.h"

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Cmd * cmd_new(int (* caller)(const char *, char * const *), char ** argv) {
    struct Cmd * cmd = malloc(sizeof(struct Cmd));

    cmd->caller = caller;

    cmd->argv0 = argv[0];
    cmd->argv = argv;

    cmd->pipes[0] = NULL;
    cmd->pipes[1] = NULL;
    cmd->pipes[2] = NULL;

    return cmd;
}

void cmd_del(struct Cmd * cmd) {
    if(cmd) {
        if(cmd->argv0 != cmd->argv[0]) {
            free(cmd->argv0);
        }
        for(char ** v = cmd->argv; *v; ++v) {
            free(*v);
        }
        free(cmd->argv);
    }
    free(cmd);
}

static inline void _cmd_set_io(int fd[2], size_t src, int des, int cls) {
    dup2(fd[src], des);
    if(cls) {
        close(fd[0]);
        close(fd[1]);
    }
}

static inline void _cmd_set_in(struct Pipe * ip, int des, int cls) {
    if(ip) {
        _cmd_set_io(ip->fd, 0, des, cls);
    }
}

static inline void _cmd_set_out(struct Pipe * op, int des, int cls) {
    if(op) {
        _cmd_set_io(op->fd, 1, des, cls);
    }
}

void cmd_set_io(struct Cmd * cmd) {
    _cmd_set_in(cmd->pipes[0], STDIN_FILENO, 1);
    _cmd_set_out(cmd->pipes[1], STDOUT_FILENO, cmd->pipes[1] != cmd->pipes[2]);
    _cmd_set_out(cmd->pipes[2], STDERR_FILENO, 1);
}

struct CmdList * cmd_list_new() {
    struct CmdList * list = malloc(sizeof(struct CmdList));
    list->line = NULL;
    list->pipes = pipe_list_new();
    list->cc = 0;
    return list;
}

void cmd_list_del(struct CmdList * list) {
    if(list) {
        free(list->line);
        pipe_list_del(list->pipes);
    }
    free(list);
}

void cmd_list_setline(struct CmdList * list, char * line) {
    char * old = list->line;
    list->line = NULL;
    if(line) {
        list->line = strdup(line);
    }
    free(old);
}

struct Cmd * cmd_list_next(struct CmdList * list) {
    int num_pipe = 0;
    char * cmd_str = NULL;
    char * next = NULL;

    if(!(cmd_str = parser_line(list->line, &num_pipe, &next))) {
        if(num_pipe) {
            write(STDERR_FILENO, "syntax error near pipe\n", 41);
        }
        return NULL;
    }

    char ** argv = parser_cmd(cmd_str);
    free(cmd_str);

    int (* caller)(const char *, char * const *) = parser_caller(argv[0]);
    struct Cmd * cmd = cmd_new(caller, argv);

    cmd->pipes[0] = pipe_list_find(list->pipes, list->cc);
    if(num_pipe) {
        cmd->pipes[1] = pipe_list_get(list->pipes, list->cc + (num_pipe & INT_MAX));
    }
    if(num_pipe < 0) {
        cmd->pipes[2] = cmd->pipes[1];
    }

    ++list->cc;
    cmd_list_setline(list, next);

    return cmd;
}

void cmd_list_remove(struct Cmd * cmd) {
    pipe_list_remove(cmd->pipes[0]);
    cmd_del(cmd);
}
