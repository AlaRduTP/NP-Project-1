#ifndef NPSHELL
#define NPSHELL

#include "cmd.h"
#include "pipe.h"

#include <sys/types.h>

struct NPShell {
    struct CmdList * cmds;
    struct PipeList * pipes;
};

struct NPShell * npshell_new();
void npshell_del(struct NPShell * shell);

ssize_t npshell_getline(struct NPShell * shell);
void npshell_exec(struct NPShell * shell);

#endif
