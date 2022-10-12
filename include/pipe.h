#ifndef NPSHELL_PIPE
#define NPSHELL_PIPE

#include <stddef.h>

#define NPS_PLSIZE 1000

struct Pipe;

struct Pipe {
    int fd[2];
    size_t id;
    struct Pipe * next;
    struct Pipe * prev;
};

struct PipeList {
    struct Pipe head[NPS_PLSIZE];   /* Dummy heads for PipeList hash table */
};

struct Pipe * pipe_new(const size_t id);
void pipe_del(struct Pipe * pp);

struct PipeList * pipe_list_new();
void pipe_list_del(struct PipeList * list);

struct Pipe * pipe_list_find(struct PipeList * list, const size_t id);
struct Pipe * pipe_list_get(struct PipeList * list, const size_t id);
void pipe_list_remove(struct Pipe * pp);

#endif
