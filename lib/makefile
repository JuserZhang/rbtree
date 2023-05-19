CC = gcc
CFLAGS = -Wall -Werror -fpic -g
LDFLAGS = -shared
LIBNAME = librbt.so
SRCS = rbtree.c rbt_api.c
OBJS = $(SRCS:.c=.o)

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(LIBNAME)
	sudo cp $(LIBNAME) /usr/lib/
	sudo chmod 0755 /usr/lib/$(LIBNAME)
	sudo ldconfig

clean:
	rm -f $(OBJS) $(LIBNAME)