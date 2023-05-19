CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS = -L. -lrbt -pthread
SRCS = test.c
OBJS = $(SRCS:.c=.o)
TARGET = test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
