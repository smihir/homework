#ifndef _MYSH_READLINE_H
#define _MYSH_READLINE_H

typedef enum {
    INPUT_READ_OK = 0,
    INPUT_READ_OVERFLOW = 1,
    INPUT_READ_EOF = 2,
    INPUT_READ_ERROR = 3,
} READ_STATUS;

READ_STATUS readInput(FILE * input, char **cmdLine);

char **parseInput();

#endif /* _MYSH_READLINE_H */
