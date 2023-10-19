#Macros function kind of like a Variable that stores commands-parts. It can be used to simplefy bigger commands and reduce repetiveness.
#They can be appended via $(nameOfMacro)

IDIR=../include
ODIR=obj
LDIR=../lib
LIBS=none										#Macro for any additional libraries that you might want to include

# Yes, Macros can be used inside other Macros
CC=gcc											#Macro for Compiler Version used

objects = hellofunc.o hellomake.o

edit : $(objects)
	gcc -o edit $(objects)

hellofunc.o: hellofunc.c
hellomake.o: hellomake.h


#%.o:	%.c $(DEPS)
#		$(CC) -c -o $@ $< $(CFLAGS)

#hellomake:	$(OBJ)
#		$(CC) -o $@ $^ $(CFLAGS)

.PHONY:	clean									#Protect files named clean from beeing cleaned by clean command (a lotta "clean" words in there, aye?
clean:											#Cleans your shit-ass fuc***..., ups, I mean: Cleans your source and object directories with command: make clean
		rm edit $(objects)