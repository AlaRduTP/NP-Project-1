#define _POSIX_C_SOURCE 200112L

#include "utils.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t nps_args_count(char * const * args) {
    size_t count = 0;
    while(args[count]) {
        ++count;
    }
    return count;
}

void nps_setenv_real(char * const * args) {
    if(nps_args_count(args) == 3) {
        setenv(args[1], args[2], 1);
    }
}

int nps_setenv(const char * self, char * const * args) {
    if(nps_args_count(args) != 3) {
        fputs("usage: setenv NAME VALUE\n", stderr);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int nps_printenv(const char * self, char * const * args) {
    if(nps_args_count(args) != 2) {
        fputs("usage: printenv NAME\n", stderr);
        exit(EXIT_FAILURE);
    }
    char * env = getenv(args[1]);
    if(env) {
        puts(env);
    }
    exit(EXIT_SUCCESS);
}

void nps_exit_real(char * const * args) {
    /* Kill all child processes if they do not change process group ID. */
    /* Maybe we can just exit? */
    // kill(0, SIGINT);
    exit(EXIT_SUCCESS);
}

int nps_exit(const char * self, char * const * args) {
    /* This should not happen */
    fputs("exit: why are you alive?\n", stderr);
    exit(EXIT_FAILURE);
}
