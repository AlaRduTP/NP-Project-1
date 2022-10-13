#ifndef NPSHELL_PARSER
#define NPSHELL_PARSER

char * parser_line(char * line, int * num_pipe, char ** next);
char ** parser_cmd(char * cmd_str);
int (* parser_caller(char * caller_str))(const char *, char * const *);
void (* parser_bfrcall(int (* caller)(const char *, char * const *)))(char * const *);

#endif
