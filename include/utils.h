#ifndef NPSHELL_UTILS
#define NPSHELL_UTILS

#include <stddef.h>

size_t nps_args_count(char * const * args);

void nps_setenv_real(char * const * args);
void nps_exit_real(char * const * args);

int nps_setenv(const char * self, char * const * args);
int nps_printenv(const char * self, char * const * args);
int nps_exit(const char * self, char * const * args);

#endif
