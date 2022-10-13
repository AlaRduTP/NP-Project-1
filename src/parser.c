#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#define _BSD_SOURCE

#include "parser.h"
#include "utils.h"

#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NPS_O_FLAG (O_CREAT | O_WRONLY | O_TRUNC)
#define NPS_O_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char * parser_line(char * line, int * num_pipe, char ** next) {
    *num_pipe = 0;
    
    while(line && *line == ' ') {
        ++line;
    }
    if(!line || !*line) {
        return NULL;
    }
    
    char * pp = strpbrk(line, "|!");
    
    if(pp) {
        if(pp == line) {
            *num_pipe = -1;
            return NULL;
        }

        if(pp[1] < '0' || pp[1] > '9') {
            *num_pipe = 1;
        } else {
            sscanf(pp + 1, "%d", num_pipe);
        }

        if(*pp == '!') {
            *num_pipe |= ~(int)INT_MAX;
        }

        *next = pp + 1;
        while(**next >= '0' && **next <= '9') {
            ++*next;
        }

        *pp = '\0';
    }

    return strdup(line);
}

char ** parser_cmd(char * cmd_str) {
    char ** argv = NULL;
    size_t argc = 0;

    while(cmd_str) {
        while(*cmd_str == ' ') {
            ++cmd_str;
        }
        if(!*cmd_str) {
            break;
        }
        char * tok = strsep(&cmd_str, " ");
        argv = realloc(argv, (argc + 1) * sizeof(char *));
        argv[argc++] = strdup(tok);
    }

    argv = realloc(argv, (argc + 1) * sizeof(char *));
    argv[argc] = NULL;
    return argv;
}

int parse_redir(char *** argv) {
    size_t argc = nps_args_count(*argv);
    if(argc < 3 || strcmp((*argv)[argc - 2], ">")) {
        return -1;
    }

    int fd = open((*argv)[argc - 1], NPS_O_FLAG, NPS_O_MODE);

    size_t new_argc = argc - 2;
    char ** new_argv = malloc((new_argc + 1) * sizeof(char *));

    for(size_t i = 0; i < new_argc; ++i) {
        new_argv[i] = (*argv)[i];
    }
    new_argv[new_argc] = NULL;

    free((*argv)[argc - 2]);
    free((*argv)[argc - 1]);

    *argv = new_argv;
    return fd;
}

int (* parser_caller(char * caller_str))(const char *, char * const *) {
    int (* caller)(const char *, char * const *) = execvp;
    
    if(!strcmp(caller_str, "setenv")) {
        caller = nps_setenv;
    }
    if(!strcmp(caller_str, "printenv")) {
        caller = nps_printenv;
    }
    if(!strcmp(caller_str, "exit")) {
        caller = nps_exit;
    }

    return caller;
}

void (* parser_bfrcall(int (* caller)(const char *, char * const *)))(char * const *) {
    void (* bfrcall)(char * const *) = NULL;

    if(caller == nps_setenv) {
        bfrcall = nps_setenv_real;
    }
    if(caller == nps_exit) {
        bfrcall = nps_exit_real;
    }

    return bfrcall;
}
