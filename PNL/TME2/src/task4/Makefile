# VERSION 1
# 
# .PHONY : all clean check
# .ONESHELL:
# 
# all : cron_func
# 
# %.o : %.c
# 	gcc -Wall -c $<
# 
# cron_func : cron_func.o nothing.o
# 	gcc -Wall $^ -o $@
# 
# clean :
# 	rm -rf *~ *.o cron_func

# VERSION 2
# 
# .PHONY: all clean
# .ONESHELL:

# CC = gcc
# CFLAGS = -Wall -fPIC
# LDFLAGS = -L. -lfunc

# all: cron_func

# # Object files
# cron_func.o: cron_func.c func.h
# 	$(CC) -Wall -c cron_func.c

# nothing.o: nothing.c func.h
# 	$(CC) $(CFLAGS) -c nothing.c

# # Shared library
# libfunc.so: nothing.o
# 	$(CC) -shared nothing.o -o libfunc.so

# # Final executable (dynamically linked)
# cron_func: cron_func.o libfunc.so
# 	$(CC) -Wall cron_func.o $(LDFLAGS) -o cron_func

# clean:
# 	rm -f *.o cron_func libfunc.so

# VERSION 3

CC = gcc
CFLAGS = -Wall

all: cron_func

cron_func.o: cron_func.c func.h
	$(CC) $(CFLAGS) -c cron_func.c

nothing.o: nothing.c func.h
	$(CC) $(CFLAGS) -c nothing.c

libfunc.a: nothing.o
	ar rcs libfunc.a nothing.o

cron_func: cron_func.o libfunc.a
	$(CC) -static cron_func.o -L. -lfunc -o cron_func

clean:
	rm -f *.o *.a cron_func
