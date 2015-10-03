#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "history.h"

ll list; 

void init_ll()
{
	list.head = 0;
	list.tail = -1;
	list.counter = 0;
}


void add_cmd(char *cmd)
{
	list.tail++;
	list.tail = list.tail % MAX_CMD_SIZE;
	strcpy(list.cmd_list[list.tail].cmd, cmd);
	list.counter++;
	list.cmd_list[list.tail].num = list.counter;
	if(list.counter > MAX_CMD_SIZE) {
		list.head++;
		list.head = list.head % MAX_CMD_SIZE;
	}
}

char *get_nth_cmd(int n)
{
    char *cmd = NULL;
    int i;
    int max = list.counter < MAX_CMD_SIZE ? list.counter : MAX_CMD_SIZE;
    for(i = 0; i < max; i++) {
        ll_node node = list.cmd_list[i];
        if(node.num == n) {
                cmd = node.cmd;
                break;
        }
    }
    return cmd;
}

char *get_last_cmd()
{
	char *cmd = NULL;
	if(list.counter > 0) {
		cmd = list.cmd_list[list.tail].cmd;
	}
	return cmd;
}
void display_all_cmd()
{
    int i;
    if (list.head < list.tail) {
        for (i = list.head; i <= list.tail; i++) {
            printf("%d %s", list.cmd_list[i].num, list.cmd_list[i].cmd);
        }

    } else if (list.head > list.tail) {
        for (i = list.head; i < MAX_CMD_SIZE; i++) {
            printf("%d %s", list.cmd_list[i].num, list.cmd_list[i].cmd);
        }
        for (i = 0; i <= list.tail; i++) {
            printf("%d %s", list.cmd_list[i].num, list.cmd_list[i].cmd);
        }
    } else {
        printf("%d %s", list.cmd_list[list.head].num, list.cmd_list[list.head].cmd);
    }
}
