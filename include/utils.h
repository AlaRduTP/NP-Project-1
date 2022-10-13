#ifndef NPSHELL_UTILS
#define NPSHELL_UTILS

void nps_setenv_real(char * const * args);

int nps_setenv(const char * self, char * const * args);
int nps_printenv(const char * self, char * const * args);
int nps_exit(const char * self, char * const * args);

#endif
