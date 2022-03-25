POOLM = ../../CI/objects/pool/poolM.o

pool: pool.c $(POOLM) bintree.o queue.o stack.o array_list.o 
	gcc --std=c18 -Ilib -g pool.c $(POOLM) bintree.o queue.o stack.o array_list.o -o pool -lpthread 

bintree.o: lib/bintree.c
	gcc --std=c18 -c -Ilib lib/bintree.c

queue.o: lib/queue.c
	gcc --std=c18 -c -Ilib lib/queue.c

stack.o: lib/stack.c
	gcc --std=c18 -c -Ilib lib/stack.c

array_list.o: lib/array_list.c
	gcc --std=c18 -c -Ilib lib/array_list.c

clean:
	rm *.o pool
