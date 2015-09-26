CC = gcc
CSCOPE = cscope
CFLAGS += -Wall -Werror

OBJS := mysh.o \
		readline.o \
		print.o \

ifeq ($(DEBUG), y)
 CFLAGS += -g -DDEBUG
endif

mysh: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o mysh

%.o: %.c *.h
	$(CC) $(CFLAGS) -o $@ -c $<

cscope:
	$(CSCOPE) -bqR

.PHONY: clean
clean:
	rm -rf *.o mysh
