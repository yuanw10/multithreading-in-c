THREADSM = ../../CI/objects/threads/threadsM.o
UNSAFEM = ../../CI/objects/threads/unsafeM.o
SAFEM = ../../CI/objects/threads/safeM.o

all: threads unsafe safe

threads: threads.c $(THREADSM)
	gcc --std=c18 -g threads.c $(THREADSM) -o threads -lpthread

unsafe: unsafe.c $(UNSAFEM)
	gcc --std=c18 -g unsafe.c $(UNSAFEM) -o unsafe -lpthread

safe: safe.c $(SAFEM)
	gcc --std=c18 -g safe.c $(SAFEM) -o safe -lpthread

clean:
	rm threads unsafe safe
