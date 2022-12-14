#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L

#include "cmd.h"
#include "npshell.h"

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern int errno;

static void _npshell_init(struct NPShell ** shell) {
    struct sigaction act = {
        .sa_flags = SA_NOCLDSTOP | SA_SIGINFO | SA_NOCLDWAIT
    };
    
    sigaction(SIGCHLD, &act, NULL);
    setenv("PATH", "bin:.", 1);

    *shell = malloc(sizeof(struct NPShell));
    (*shell)->cmds = cmd_list_new();
}

struct NPShell * npshell_new() {
    /* Singleton */
    static struct NPShell * shell = NULL;
    
    if(!shell) {
        _npshell_init(&shell);
    }

    return shell;
}

void npshell_del(struct NPShell * shell) {
    if(shell) {
        cmd_list_del(shell->cmds);
    }
    free(shell);
}

ssize_t npshell_getline(struct NPShell * shell) {
    write(STDOUT_FILENO, "% ", 2);

    char * line = NULL;
    size_t linecap = 0;
    ssize_t linelen = getline(&line, &linecap, stdin);

    if(line[linelen - 1] == '\n') {
        line[linelen - 1] = '\0';
    }

    cmd_list_setline(shell->cmds, line);
    free(line);

    return linelen;
}

static void _npshell_exec(struct Cmd * cmd) {
    if(cmd->bfrcall) {
        cmd->bfrcall(cmd->argv);
    }

    pid_t pid;
    while((pid = fork()) == -1) {

        // TBD: fork failed

    }
    if(pid == 0) {
        /* child */
        cmd_set_io(cmd);
        cmd->caller(cmd->argv0, cmd->argv);

        if(errno == ENOENT) {
            fprintf(stderr, "Unknown command: [%s].\n", cmd->argv0);
        } else {
            perror(cmd->argv0);
        }
        _exit(EXIT_FAILURE);
    } else {
        /* parent */
    }
}

void npshell_exec(struct NPShell * shell) {
    struct Cmd * cmd;
    int block = 0;
    while(cmd = cmd_list_next(shell->cmds)) {
        block = cmd->pipes[1] == NULL;
        _npshell_exec(cmd);
        cmd_list_remove(cmd);
    }
    while(block && wait(NULL) > 0) { /* nop */ }
}
