
#ifndef _MYSH_BUILTIN_H
#define _MYSH_BUILTIN_H

extern char *re_exec_cmd;

int is_builtin(char** shArgs);

int do_builtin(char **shArgs);

int check_re_exec(char **shArgs);

#endif /* _MYSH_BUILTIN_H */
