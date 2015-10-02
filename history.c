#include<stdlib.h>
#include<string.h>
#include "history.h"

ll list; 

void init_ll()
{
	list.head = 0;
	list.tail = 0;
	list.counter = 0;
}


void add_cmd(char *cmd)
{
	ll_node node;
	strcpy(node.cmd, cmd);
	node.num = strlen(cmd);
	list.tail++;
	list.tail = list.tail % MAX_CMD_SIZE;
	list.cmd_list[list.tail] = node;
	list.counter++;
	if(list.counter >= MAX_CMD_SIZE) {
		list.head++;
		list.head = list.head % MAX_CMD_SIZE;
	}
}

char *get_nth_cmd(int n)
{
	return NULL;
}

void get_all_cmd()
{

}
