#ifndef NPSHELL_CMD
#define NPSHELL_CMD

#include "pipe.h"

#include <stddef.h>
#include <sys/types.h>

struct Cmd {
    /* * *
     * Caller of the command, usually execvp.
     * This function should return only if an error has occurred.
     */
    int (* caller)(const char *, char * const *);

    /* * *
     * bfrcall() should be called before caller() if it is not NULL.
     */
    void (* bfrcall)(char * const *);

    char * argv0;
    char ** argv;

    struct Pipe * pipes[3];
    int redir[3];
};

struct CmdList {
    /* * *
     * Input to be parse.
     * Shall be NULL if all commands are complete.
     */
    char * line;

    struct PipeList * pipes;
    size_t cc;  /* Command counter */
};

struct Cmd * cmd_new(int (* caller)(const char *, char * const *), char ** argv);
void cmd_del(struct Cmd * cmd);

void cmd_set_io(struct Cmd * cmd);

struct CmdList * cmd_list_new();
void cmd_list_del(struct CmdList * list);

void cmd_list_setline(struct CmdList * list, char * line);
struct Cmd * cmd_list_next(struct CmdList * list);
void cmd_list_remove(struct Cmd * cmd);

#endif
