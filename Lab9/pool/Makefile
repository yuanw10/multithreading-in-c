POOLM = ../../CI/objects/pool/poolM.o

pool: pool.c $(POOLM) queue.o heap.o array_list.o
	gcc --std=c18 -Ilib -g pool.c $(POOLM) queue.o heap.o array_list.o -o pool -lpthread 

queue.o: lib/queue.c
	gcc --std=c18 -c -Ilib lib/queue.c

heap.o: lib/heap.c
	gcc --std=c18 -c -Ilib lib/heap.c

array_list.o: lib/array_list.c
	gcc --std=c18 -c -Ilib lib/array_list.c

clean:
	rm *.o pool
