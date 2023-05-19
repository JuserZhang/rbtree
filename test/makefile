CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -L. -lrbt -pthread
SRCS = test.c
OBJS = $(SRCS:.c=.o)
TARGET = test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
