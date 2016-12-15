CC=gcc
RELEASE=-O3 -fstrict-aliasing -flto
DEBUG=-g -O0
CFLAGS=-Wall -Werror -pedantic $(DEBUG) -std=c11
OBJS = lexer.o main.o
TARGET=interpreter

$(TARGET): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS)
main.o: main.c
	$(CC) -o $@ $(CFLAGS) -c $<
%.o: %.c %.h
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)

