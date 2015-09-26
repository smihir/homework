CFLAGS += -Wall -Werror 
CC = gcc

ifeq ($(DEBUG), y)
 CFLAGS += -g -DDEBUG
endif

mysh: mysh.o

mysh.o: mysh.c
	$(CC) -c $(CFLAGS) mysh.c

.PHONY: clean
clean:
	rm -rf *.o mysh
