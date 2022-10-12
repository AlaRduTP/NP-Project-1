#include "pipe.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

struct Pipe * pipe_new(const size_t id) {
    struct Pipe * pp = malloc(sizeof(struct Pipe));
    pipe(pp->fd);
    pp->id = id;
    pp->next = pp;
    pp->prev = pp;
    return pp;
}

void pipe_del(struct Pipe * pp) {
    if(pp) {
        close(pp->fd[0]);
        close(pp->fd[1]);
    }
    free(pp);
}

struct PipeList * pipe_list_new() {
    struct PipeList * list = malloc(sizeof(struct PipeList));
    for(size_t i = 0; i < NPS_PLSIZE; ++i) {
        list->head[i].next = list->head + i;
        list->head[i].prev = list->head[i].next;
    }
    return list;
}

void pipe_list_del(struct PipeList * list) {
    for(size_t i = 0; i < NPS_PLSIZE; ++i) {
        struct Pipe * head = list->head + i;
        while(head->next != head) {
            pipe_list_remove(head->next);
        }
    }
    free(list);
}

struct Pipe * pipe_list_find(struct PipeList * list, const size_t id) {
    struct Pipe * pp = list->head + id % NPS_PLSIZE;
    for(pp = pp->next; pp; pp = pp->next) {
        if(pp->id == id) {
            break;
        }
    }
    return pp;
}

static struct Pipe * _pipe_list_add(struct PipeList * list, const size_t id) {
    struct Pipe * pp = pipe_new(id);
    struct Pipe * head = list->head + id % NPS_PLSIZE;
    pp->next = head;
    pp->prev = head->prev;
    head->prev = pp;
    pp->prev->next = pp;
    return pp;
}

struct Pipe * pipe_list_get(struct PipeList * list, const size_t id) {
    struct Pipe * pp = pipe_list_find(list, id);
    if(!pp) {
        pp = _pipe_list_add(list, id);
    }
    return pp;
}

void pipe_list_remove(struct Pipe * pp) {
    pp->prev->next = pp->next;
    pp->next->prev = pp->prev;
    pipe_del(pp);
}
