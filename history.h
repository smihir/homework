#ifndef _MYSH_HISTORY_H
#define _MYSH_HISTORY_H

#define MAX_CMD_SIZE 20

typedef struct _ll_node {
	char cmd[513];
	int num;
}ll_node;

typedef struct _ll{
	
	ll_node *head;
	ll_node *tail;
	ll_node cmd_list[MAX_CMD_SIZE]; 
}ll;

void init_ll();

void add_cmd(char *cmd);

char *get_nth_cmd(int n);

void get_all_cmd();

#endif /* _MYSH_HISTORY_H */
