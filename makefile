BIN=lib_rbtree.so
CC=gcc
CFLAGS = -shared -fPIC
LIBS=

INCS=-I./../inc/ #头文件目录
SRCS:=$(wildcard *.c) #.c文件目录
COBJS:=$(SRCS:.c=.o)

all:$(BIN)

$(COBJS) : %.o: %.c
	$(CC) -c $< -o $@ $(INCS) $(CFLAGS)

$(BIN):$(COBJS)
	$(CC) -o $(BIN) $(COBJS) $(LIBS) $(CFLAGS)

clean:
	rm $(BIN) $(COBJS)
