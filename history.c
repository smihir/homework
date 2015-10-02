#include<stdlib.h>
#include<string.h>
#include<stdio.h>
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
        for (i = 0; i < list.tail; i++) {
            printf("%d %s", list.cmd_list[i].num, list.cmd_list[i].cmd);
        }
    } else {
        printf("Error in history! head=tail \n");
    }
}
