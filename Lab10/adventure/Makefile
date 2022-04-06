adventure_main: adventure_main.c simulation.c locations.o heroes.o heap.o queue.o array_list.o
	gcc --std=c18 -g -Ilib simulation.c adventure_main.c locations.o heroes.o heap.o queue.o array_list.o -o adventure_main -lpthread

locations.o: locations.c
	gcc --std=c18 -g -c -Ilib locations.c

heroes.o: heroes.c
	gcc --std=c18 -g -c -Ilib heroes.c

heap.o: lib/heap.c
	gcc --std=c18 -g -c -Ilib lib/heap.c

queue.o: lib/queue.c
	gcc --std=c18 -g -c -Ilib lib/queue.c

array_list.o: lib/array_list.c
	gcc --std=c18 -g -c -Ilib lib/array_list.c

clean:
	rm *.o adventure_main
