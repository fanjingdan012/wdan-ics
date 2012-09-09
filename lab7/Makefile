#
# Makefile for the malloc lab test harness
#
CC = gcc
CFLAGS = -Wall -O2

OBJS = mtest.o mm.o memlib.o fsecs.o fcyc.o clock.o ftimer.o

mtest: $(OBJS)
	$(CC) $(CFLAGS) -o mtest $(OBJS)

mtest.o: mtest.c fsecs.h fcyc.h clock.h memlib.h config.h mm.h
memlib.o: memlib.c memlib.h
mm.o: mm.c mm.h memlib.h
fsecs.o: fsecs.c fsecs.h config.h
fcyc.o: fcyc.c fcyc.h
ftimer.o: ftimer.c ftimer.h config.h
clock.o: clock.c clock.h

clean:
	rm -f *~ *.o mtest

