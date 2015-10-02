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
