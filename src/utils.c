#define _POSIX_C_SOURCE 200112L

#include "utils.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static size_t _nps_args_count(char * const * args) {
    size_t count = 0;
    while(args[count]) {
        ++count;
    }
    return count;
}

void nps_setenv_real(char * const * args) {
    if(_nps_args_count(args) == 3) {
        setenv(args[1], args[2], 1);
    }
}

int nps_setenv(const char * self, char * const * args) {
    if(_nps_args_count(args) != 3) {
        fputs("usage: setenv NAME VALUE\n", stderr);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int nps_printenv(const char * self, char * const * args) {
    if(_nps_args_count(args) != 2) {
        fputs("usage: printenv NAME\n", stderr);
        exit(EXIT_FAILURE);
    }
    char * env = getenv(args[1]);
    if(env) {
        puts(env);
    }
    exit(EXIT_SUCCESS);
}

int nps_exit(const char * self, char * const * args) {
    kill(getppid(), SIGINT);
}
