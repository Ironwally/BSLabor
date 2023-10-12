CC=gcc
CFLAGS=-I -g
DEPS=hellomake.h

%.o:	%.c $(DEPS)
		$(CC) -o -c $@ $< $(CFLAGS)

hellomake:	hellomake.o hellofunc.o
		$(CC) -o hellomake hellomake.o hellofunc.o
