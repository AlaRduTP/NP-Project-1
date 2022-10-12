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

int nps_setenv(const char * self, char * const * args) {
    if(_nps_args_count(args) < 3) {
        perror("usage: setenv NAME VALUE");
        exit(EXIT_FAILURE);
    }
    setenv(args[1], args[2], 1);
    exit(EXIT_SUCCESS);
}

int nps_printenv(const char * self, char * const * args) {
    if(_nps_args_count(args) < 2) {
        perror("usage: printenv NAME ");
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
