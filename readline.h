#ifndef _MYSH_READLINE_H
#define _MYSH_READLINE_H

#define MAX_LENGTH 514
#define TOKEN_DELIM " \t\n\r"

typedef enum {
    INPUT_READ_OK = 0,
    INPUT_READ_OVERFLOW = 1,
    INPUT_READ_EOF = 2,
    INPUT_READ_ERROR = 3,
} READ_STATUS;

typedef enum {
    REDIR_ERROR = 0,
    REDIR_OK_REDIR = 1,
    REDIR_OK_NOREDIR = 2,
} REDIR_PARSE_STATUS;

extern char * redir_file;

READ_STATUS readInput(FILE * input, char **cmdLine);

REDIR_PARSE_STATUS check_redirection(char *cmdLine);

char **parseInput();

#endif /* _MYSH_READLINE_H */
