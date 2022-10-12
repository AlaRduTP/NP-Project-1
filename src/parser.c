#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include "parser.h"
#include "utils.h"

#include <limits.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
        char * tok = strsep(&cmd_str, " ");
        argv = realloc(argv, (argc + 1) * sizeof(char *));
        argv[argc++] = strdup(tok);
    }

    argv = realloc(argv, (argc + 1) * sizeof(char *));
    argv[argc] = NULL;
    return argv;
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
