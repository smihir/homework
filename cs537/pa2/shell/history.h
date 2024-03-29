#ifndef _MYSH_HISTORY_H
#define _MYSH_HISTORY_H

#define MAX_CMD_SIZE 20

typedef enum {
    HISTORY_EXIT = 0,
    HISTORY_HISTORY = 1,
    HISTORY_HISTORY_EXPANSION = 2,
} HISTORY_CMD;

typedef struct _ll_node {
	char cmd[513];
	int num;
}ll_node;

typedef struct _ll{
	int head;
	int tail;
	int counter;
	ll_node cmd_list[MAX_CMD_SIZE]; 
}ll;

void init_ll();

void add_cmd(char *cmd);

char *get_nth_cmd(int n);

char *get_last_cmd();

void display_all_cmd();

#endif /* _MYSH_HISTORY_H */
